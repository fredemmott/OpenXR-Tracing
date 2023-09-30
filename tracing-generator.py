# MIT License
#
# Copyright(c) 2021-2022 Matthieu Bucchianeri
# Copyright(c) 2023 Fred Emmott
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this softwareand associated documentation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions :
#
# The above copyright noticeand this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os
import re
import sys

# Import dependencies from the OpenXR SDK.
cur_dir = os.path.abspath(os.path.dirname(__file__))  # nopep8
base_dir = cur_dir  # nopep8
sdk_dir = os.path.join(base_dir, "third-party", "OpenXR-SDK-Source")  # nopep8
sys.path.append(os.path.join(sdk_dir, "specification", "scripts"))  # nopep8
sys.path.append(os.path.join(sdk_dir, "src", "scripts"))  # nopep8

from collections import namedtuple
from xrconventions import OpenXRConventions
from generator import write
from reg import Registry
from automatic_source_generator import (
    AutomaticSourceOutputGenerator,
    AutomaticSourceGeneratorOptions,
)


class BoilerplateOutputGenerator(AutomaticSourceOutputGenerator):
    """Common generator utilities and formatting."""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._next_structs = {}
        self.ConstantData = namedtuple("ConstantData", ["name", "value"])
        self.api_constants = []
        self._fixed_array_limit = 16

    def hasNextDumper(self, xr_struct):
        if not [member for member in xr_struct.members if member.name == "next"]:
            return False
        nexts = self.getNextStructRelationGroupForBaseStruct(xr_struct.name)
        if nexts is not None:
            return True
        for group in self._struct_relation_groups.values():
            if xr_struct.name in group.child_struct_names:
                # We should be iterating over the parent, so
                # don't double-print, and hopefully save some
                # cl.exe memory...
                return False
        return True

    def getNextDumperDecl(self, xr_struct):
        if not self.hasNextDumper(xr_struct):
            return None
        return f"inline void OXRTL_DUMP_{xr_struct.name}_NEXT(::TraceLoggingActivity<::OXRTracing::gTraceProvider>& oxrtlActivity, const char*, const {xr_struct.name}& oxrtlIt)"

    def genEnum(self, enum_info, name, alias):
        # genEnum() is called for C constants
        # genGroup() is called for a group of C constants (a C enum)
        super().genEnum(enum_info, name, alias)
        if alias:
            return
        if enum_info.elem.get("extends"):
            return
        self.api_constants.append(
            self.ConstantData(name=name, value=enum_info.elem.get("value"))
        )

    def genGroup(self, group_info, name, alias):
        super().genGroup(group_info, name, alias)
        if alias:
            return
        elem = group_info.elem
        if elem.get("supported") == "disabled":
            return
        if elem.get("type") not in ("enum", "bitmask"):
            return
        for enum in elem.findall("enum"):
            self.api_constants.append(
                self.ConstantData(name=enum.get("name"), value=enum.get("value"))
            )

    def genStructUnion(self, type_info, type_category, type_name, alias):
        super().genStructUnion(type_info, type_category, type_name, alias)
        if type_category != "struct":
            return
        next_of = type_info.elem.get("structextends")
        if not next_of:
            return
        group = self._next_structs.get(next_of)
        if not group:
            group = self.StructRelationGroup(
                generic_struct_name=next_of, child_struct_names=[]
            )
            self._next_structs[next_of] = group
        group.child_struct_names.append(type_name)

    def getNextStructRelationGroupForBaseStruct(self, name):
        return self._next_structs.get(name)

    def outputGeneratedHeaderWarning(self):
        warning = """// *********** THIS FILE IS GENERATED - DO NOT EDIT ***********"""
        write(warning, file=self.outFile)

    def outputCopywriteHeader(self):
        copyright = """// MIT License
//
// Copyright(c) 2021-2022 Matthieu Bucchianeri
// Copyright(c) 2023 Fred Emmott
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
"""
        write(copyright, file=self.outFile)

    def isEmptyStruct(self, name):
        if self.getRelationGroupForBaseStruct(name) is not None:
            return False
        for xr_struct in self.api_structures:
            if xr_struct.name != name:
                continue
            members = [
                it
                for it in xr_struct.members
                if it.name != "next" and it.name != "type"
            ]
            return not members
        return False

    def getWrappedCommands(self):
        handwritten = ["xrGetInstanceProcAddr", "xrCreateInstance"]
        skip = handwritten + self.no_trampoline_or_terminator
        all = self.core_commands + self.ext_commands
        all.sort(key=lambda command: command.name)
        return [command for command in all if command.name not in skip]

    def outputGeneratedAuthorNote(self):
        pass


class MacroOutputGenerator(BoilerplateOutputGenerator):
    def genPlatformTypeMacros(self):
        names = [
            "float",
            "int16_t",
            "int32_t",
            "int64_t",
            "int8_t",
            "size_t",
            "uint16_t",
            "uint32_t",
            "uint64_t",
            "uint8_t",
            "uintptr_t",
        ]
        macros = []
        for name in names:
            macros.append(
                f"#define OXRTL_ARGS_{name}(oxrtlIt, oxrtlName) TraceLoggingValue((static_cast<{name}>(oxrtlIt)), oxrtlName)"
            )
        for name in names:
            macros.append(
                f"""
#define OXRTL_DUMP_{name}(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt)
    TraceLoggingWriteTagged(
        oxrtlActivity,
        oxrtlName,
        OXRTL_ARGS_{name}(oxrtlIt, oxrtlValueName));
""".strip().replace(
                    "\n", "\\\n"
                )
            )
        return "\n".join(macros)

    def genBaseTypeMacros(self):
        handwritten = {"XrAction", "XrActionSet", "XrSpace"}

        ret = ""
        for xr_type in sorted(self.api_base_types, key=lambda xr_type: xr_type.name):
            ret += self.genBaseTypeMacro(xr_type) + "\n"
        for xr_type in sorted(self.api_handles, key=lambda xr_type: xr_type.name):
            macro = (
                f"#define OXRTL_ARGS_{xr_type.name}(oxrtlIt, name) OXRTL_ARGS_HANDLE(oxrtlIt, name)"
                + "\n"
            )
            if xr_type.name in handwritten:
                ret += "// EXCLUDED - HANDWRITTEN:\n// " + macro
            else:
                ret += macro
        for xr_type in sorted(self.api_flags, key=lambda xr_type: xr_type.name):
            ret += (
                f"#define OXRTL_ARGS_{xr_type.name}(oxrtlIt, name) OXRTL_ARGS_{xr_type.type}(oxrtlIt, name)"
                + "\n"
            )

        for xr_type in sorted(
            self.api_base_types + self.api_handles, key=lambda xr_type: xr_type.name
        ):
            ret += (
                f"""
#define OXRTL_DUMP_{xr_type.name}(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt)
    TraceLoggingWriteTagged(
        oxrtlActivity,
        oxrtlName,
        OXRTL_ARGS_{xr_type.name}(oxrtlIt, oxrtlValueName));
""".strip().replace(
                    "\n", "\\\n"
                )
                + "\n"
            )
        return ret

    def genBaseTypeMacro(self, xr_type):
        handwritten_types = {"XrVersion", "XrPath"}
        if xr_type.type == "XR_DEFINE_ATOM":
            ret = f"#define OXRTL_ARGS_{xr_type.name}(oxrtlIt, name) OXRTL_ARGS_ATOM(oxrtlIt, name)"
        else:
            ret = f"#define OXRTL_ARGS_{xr_type.name}(oxrtlIt, name) OXRTL_ARGS_{xr_type.type}(oxrtlIt, name)"
        if xr_type.name in handwritten_types:
            return "// EXCLUDED - HANDWRITTEN:\n// " + ret
        return ret

    def genEnumMacros(self):
        ret = ""
        for xr_enum in sorted(self.api_enums, key=lambda xr_enum: xr_enum.name):
            ret += self.genEnumMacro(xr_enum) + "\n"
        return ret

    def genEnumMacro(self, xr_enum):
        newline = "\n"
        values = [value for value in xr_enum.values if value.alias is None]
        return f"""
namespace OXRTracing {{
inline std::string to_string({xr_enum.name} value) {{
    switch(value) {{
        {newline.join([f'case {x.name}: return "{x.name}";' for x in values])}
        default:
            using BasicT = std::underlying_type_t<{xr_enum.name}>;
            const auto basicValue = static_cast<BasicT>(value);
            return std::format("Unknown {xr_enum.name}: {{}}", basicValue);
    }}
}}
}}
#define OXRTL_ARGS_{xr_enum.name}(oxrtlIt, oxrtlName) TraceLoggingValue(OXRTracing::to_string(oxrtlIt).c_str(), oxrtlName)
#define OXRTL_DUMP_{xr_enum.name}(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt) \\
  TraceLoggingWriteTagged(oxrtlActivity, oxrtlName, OXRTL_ARGS_{xr_enum.name}(oxrtlIt, oxrtlValueName));
""".strip()

    def genDumperAliases(self):
        ret = ""
        for type_name in sorted(self.aliases):
            alias = self.aliases[type_name]
            if not self.isStruct(alias):
                continue
            ret += (
                f"""
// {type_name} = {alias}
#define OXRTL_ARGS_{type_name} OXRTL_ARGS_{alias}
#define OXRTL_DUMP_{type_name} OXRTL_DUMP_{alias}
""".strip()
                + "\n"
            )
        return ret

    def genStructMacros(self):
        ret = ""
        for xr_struct in sorted(
            self.api_structures, key=lambda xr_struct: xr_struct.name
        ):
            ret += self.genStructMacro(xr_struct) + "\n"
        return ret

    def genStructNextDumpers(self):
        ret = ""
        for xr_struct in sorted(
            self.api_structures, key=lambda xr_struct: xr_struct.name
        ):
            if self.hasNextDumper(xr_struct):
                ret += self.genStructNextDumper(xr_struct) + "\n"
        return ret

    def genStructMacro(self, xr_struct):
        handwritten = {"XrEventDataBuffer"}
        member_macros = []
        for member in xr_struct.members:
            if member.name == "next":
                continue
            if (
                member.name == "type"
                and member.type == "XrStructureType"
                and self.getRelationGroupForBaseStruct(xr_struct.name) is None
            ):
                continue
            if self.isEmptyStruct(member.type):
                continue
            if self.isBinaryMember(member):
                continue
            suffix = ""
            trailing = ""
            value = f"(oxrtlIt.{member.name})"
            pointer_count = member.pointer_count
            if member.is_array:
                if member.is_static_array:
                    if self.isStruct(member.type):
                        size = self.getFixedArraySize(member)
                        if size > self._fixed_array_limit:
                            member_macros.append(
                                f'TraceLoggingValue("array<{member.type},{size}>", oxrtlName "_{member.name}")'
                            )
                        continue  # handled by complex member dumping
                    suffix += "_FA"
                    trailing += f", {member.static_array_sizes[0]}"
                elif member.type == "char":
                    if member.pointer_count_var:
                        suffix += "_P_DA"
                        pointer_count -= 2
                        trailing += f", oxrtlIt.{member.pointer_count_var}"
                    else:
                        suffix += "_P"
                        assert (
                            member.is_null_terminated
                        ), f"string '{xr_struct.name}::{member.name}' does not have a length, and is not null-terminated"
                else:
                    continue
            if pointer_count > 0:
                value = f"({'*' * pointer_count}{value})"
            member_macros.append(
                f'OXRTL_ARGS_{member.type}{suffix}({value}, "{member.name}"{trailing})'
            )
        if xr_struct.name in handwritten:
            struct_def = f"// EXCLUDED - HANDWRITTEN: #define OXRTL_ARGS_{xr_struct.name}(oxrtlIt, oxrtlName)"
        elif member_macros:
            struct_def = f'#define OXRTL_ARGS_{xr_struct.name}(oxrtlIt, oxrtlName) TraceLoggingStruct({len(member_macros)}, oxrtlName),{", ".join(member_macros)}'
        else:
            struct_def = f"#define OXRTL_ARGS_{xr_struct.name}(oxrtlIt, oxrtlName) TraceLoggingValue(oxrtlName)"
        return f"""
{struct_def}
{self.genDumpStructMacro(xr_struct)}
"""

    def genStructNextDumper(self, xr_struct):
        func_decl = self.getNextDumperDecl(xr_struct)
        if not func_decl:
            return ""
        nexts = self.getNextStructRelationGroupForBaseStruct(xr_struct.name)
        if nexts is not None:
            nexts = nexts.child_struct_names.copy()
        else:
            nexts = []
        it = f"oxrtlNextIt_{xr_struct.name}"
        cases = []
        if nexts:
            for name in nexts:
                child_struct = self.getStruct(name)
                if not child_struct:
                    continue
                type_param = [p for p in child_struct.members if p.name == "type"][0]
                xr_type_value = type_param.values
                cases.append(
                    f"""
    case {xr_type_value}: {{
            const auto {it}_as_{child_struct.name} = *reinterpret_cast<const {child_struct.name}*>({it});
            OXRTL_DUMP_{child_struct.name}(
                oxrtlActivity,
                "{child_struct.name}_next",
                "{xr_type_value}",
                {it}_as_{child_struct.name});
        }}
        continue;
""".strip()
                )

        if not cases:
            return f"""
{func_decl} {{
    for (
        auto {it} = reinterpret_cast<const XrBaseInStructure*>(oxrtlIt.next);
        {it};
        {it} = reinterpret_cast<const XrBaseInStructure*>({it}->next)) {{
        TraceLoggingWriteTagged(
            oxrtlActivity,
            "{xr_struct.name}_next",
            OXRTL_ARGS_XrStructureType(({it}->type), "type"));
    }}
}}
""".strip()
        return f"""
{func_decl} {{
    for (
        auto {it} = reinterpret_cast<const XrBaseInStructure*>(oxrtlIt.next);
        {it};
        {it} = reinterpret_cast<const XrBaseInStructure*>({it}->next)) {{
        switch ({it}->type) {{
            {' '.join(cases)}
            default:
                TraceLoggingWriteTagged(
                    oxrtlActivity,
                    "{xr_struct.name}_next",
                    OXRTL_ARGS_XrStructureType(({it}->type), "type"));
        }}
    }}
}}
""".strip()

    def isBinaryMember(self, xr_member):
        if not xr_member.is_array:
            return False
        if xr_member.type == "uint8_t":
            return True
        if xr_member.type != "char":
            return False
        if xr_member.name in {"buffer", "varying", "bytes"}:
            return True
        return False

    def genDumpComplexMember(self, xr_member):
        if self.isBinaryMember(xr_member):
            return None
        if xr_member.is_array:
            if xr_member.is_static_array:
                if not self.isStruct(xr_member.type):
                    return None  # Handled by simple dumping
                return self.genDumpFixedArrayMember(xr_member)
            if xr_member.type == "char":
                return None  # Handled by simple dumping
            return self.genDumpDynamicArrayMember(xr_member)
        return None

    def getFixedArraySize(self, xr_member):
        size = xr_member.static_array_sizes[0]
        for xr_constant in self.api_constants:
            if xr_constant.name == size:
                return int(xr_constant.value)
        return int(size)

    def genDumpFixedArrayMember(self, xr_member):
        size = self.getFixedArraySize(xr_member)
        if size > self._fixed_array_limit:
            # Shown as a simple field instead
            return None

        ret = ""
        for i in range(0, size - 1):
            ret += f"""
OXRTL_DUMP_{xr_member.type}(
    oxrtlActivity,
    oxrtlName "_{xr_member.name}",
    "#{i}",
    ({'*' * (xr_member.pointer_count - 1)}oxrtlIt.{xr_member.name}[{i}]));
""".strip().replace(
                "\n", "\\\n"
            )
        return ret

    def genDumpDynamicArrayMember(self, xr_member):
        index_type = f"decltype(oxrtlIt.{xr_member.pointer_count_var})"
        i = f"{xr_member.type}_i"

        return f"""
{{
    for ({index_type} {i} = 0; {i} < oxrtlIt.{xr_member.pointer_count_var}; ++{i}) {{
        OXRTL_DUMP_{xr_member.type}(
            oxrtlActivity,
            oxrtlName "_{xr_member.name}",
            "element",
            ({'*' * (xr_member.pointer_count - 1)}oxrtlIt.{xr_member.name}[{i}]));
    }}
}}
""".strip().replace(
            "\n", "\\\n"
        )

    def genChildStructCase(self, xr_struct):
        type_param = [it for it in xr_struct.members if it.name == "type"][0]
        xr_type_value = type_param.values
        return f"""
case {xr_type_value}:
    OXRTL_DUMP_{xr_struct.name}(
            oxrtlActivity,
            oxrtlName,
            "{xr_type_value}",
            (*reinterpret_cast<const {xr_struct.name}*>(&oxrtlIt)));
    break;
""".strip()

    def genDumpStructMacro(self, xr_struct):
        complex_fields = []
        for member in xr_struct.members:
            member_dumper = self.genDumpComplexMember(member)
            if member_dumper is not None:
                complex_fields.append(member_dumper.strip())
        ret = f"#define OXRTL_DUMP_{xr_struct.name}_COMPLEX_FIELDS(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt)"
        if complex_fields:
            ret += "\\\n".join(complex_fields)
        if self.hasNextDumper(xr_struct):
            ret += (
                "\\\n"
                + f"OXRTL_DUMP_{xr_struct.name}_NEXT(oxrtlActivity, oxrtlName, oxrtlIt);"
            )
        ret += "\n"
        ret += f"#define OXRTL_DUMP_{xr_struct.name}(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt)"
        dump_base = f"""
    TraceLoggingWriteTagged(
        oxrtlActivity,
        oxrtlName,
        OXRTL_ARGS_{xr_struct.name}(oxrtlIt, oxrtlValueName));
    OXRTL_DUMP_{xr_struct.name}_COMPLEX_FIELDS(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt);
""".strip()
        relations = self.getRelationGroupForBaseStruct(xr_struct.name)
        if relations is None:
            ret += dump_base.replace("\n", "\\\n")
            return ret
        descendants = []
        to_visit = relations.child_struct_names.copy()
        while to_visit:
            name = to_visit.pop()
            struct = self.getStruct(name)
            descendants.append(struct)
            relations = self.getRelationGroupForBaseStruct(name)
            if relations:
                to_visit += relations.child_struct_names
        ret += f"""
switch(oxrtlIt.type) {{
{''.join([
self.genChildStructCase(xr_struct)
for xr_struct in descendants
])}
  default:
    {dump_base}
}}""".strip().replace(
            "\n", "\\\n"
        )
        return ret

    def beginFile(self, genOpts):
        BoilerplateOutputGenerator.beginFile(self, genOpts)
        write(
            """
#pragma once

#include <OXRTracing/preamble.hpp>
#include <OXRTracing/macros.hpp>
#include <OXRTracing/forward_declarations.hpp>
#include <OXRTracing/forward_declarations.gen.hpp>

#include <format>
""",
            file=self.outFile,
        )

    def endFile(self):
        contents = f"""
///////////////////////////////////////////////
///// Generated macros for platform types /////
///////////////////////////////////////////////

{self.genPlatformTypeMacros()}

//////////////////////////////////////////////////
///// Generated macros for OpenXR base types /////
//////////////////////////////////////////////////

{self.genBaseTypeMacros()}

/////////////////////////////////////////////
///// Generated macros for OpenXR enums /////
/////////////////////////////////////////////

{self.genEnumMacros()}

///////////////////////////////////////////////
///// Generated macros for OpenXR structs /////
///////////////////////////////////////////////

{self.genStructMacros()}

//////////////////////////////////////////////////////
///// Generated *next dumpers for OpenXR structs /////
//////////////////////////////////////////////////////

{self.genStructNextDumpers()}

////////////////////////////////////////
///// Generated macros for aliases /////
////////////////////////////////////////

{self.genDumperAliases()}
"""
        write(contents, file=self.outFile)
        BoilerplateOutputGenerator.endFile(self)


class LayerOutputGenerator(BoilerplateOutputGenerator):
    def __init__(self, file_number, file_count, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._file_number = file_number
        self._file_count = file_count

    def genNextPFNDefinitions(self):
        if self._file_number != 0:
            return ""
        ret = "namespace OXRTracing {\n"
        for xr_command in self.getWrappedCommands():
            ret += f"PFN_{xr_command.name} next_{xr_command.name} {{nullptr}};" + "\n"
        return ret + "\n}"

    def genXrGetInstanceProcAddr(self):
        if self._file_number != 0:
            return ""
        ret = """
XrResult XRAPI_CALL OXRTracing_xrGetInstanceProcAddr(
    XrInstance instance,
    const char* nameCStr,
    PFN_xrVoidFunction* function) {

    *function = nullptr;
    const auto ret = gXrNextGetInstanceProcAddr(instance, nameCStr, function);
    if (XR_FAILED(ret) || !*function) {
        return ret;
    }
    const std::string_view name {nameCStr};
"""
        for xr_command in self.getWrappedCommands():
            ret += f"""
if (name == "{xr_command.name}") {{
    next_{xr_command.name} = reinterpret_cast<PFN_{xr_command.name}>(*function);
    *function = reinterpret_cast<PFN_xrVoidFunction>(&OXRTracing_{xr_command.name});
    return ret;
}}
"""
        ret += """
    return ret;
}"""
        return ret

    def shouldIncludeFunction(self, name):
        return (hash(name) % self._file_count) == self._file_number

    def genWrappers(self):
        ret = ""
        for xr_command in self.getWrappedCommands():
            if xr_command.name in self.no_trampoline_or_terminator:
                continue
            if not self.shouldIncludeFunction(xr_command.name):
                continue
            ret += self.genWrapper(xr_command)
        return ret

    def genTraceStruct(self, command_name, xr_param, top_level=True):
        struct_name = xr_param.type
        return f"""
OXRTL_DUMP_{struct_name}(
    localActivity,
    "{command_name}",
    "{xr_param.name}",
    (*{xr_param.name})
);
""".strip()

    def genWrapper(self, xr_command):
        hooked = {
            "xrCreateActionSet",
            "xrCreateAction",
            "xrStringToPath",
            "xrCreateActionSpace",
            "xrCreateReferenceSpace",
        }
        newline = "\n"

        parameters = []
        arguments = []
        trace_in = [f'"{xr_command.name}"']
        trace_out = [f'"{xr_command.name}"', 'OXRTL_ARGS_XrResult(ret, "XrResult")']
        trace_next_in = []
        trace_next_out = []
        for param in xr_command.params:
            parameters.append(param.cdecl.strip())
            arguments.append(param.name)

            if self.isEmptyStruct(param.type):
                continue

            # Figure out what/how to trace
            if param.is_array or param.pointer_count > 1:
                continue
            if param.pointer_count == 0:
                trace_arg = f'OXRTL_ARGS_{param.type}({param.name}, "{param.name}")'
                trace_in.append(trace_arg)
                continue
            is_struct = self.isStruct(param.type)
            if param.type == "char":
                trace_arg = f'OXRTL_ARGS_{param.type}_P({param.name}, "{param.name}")'
            else:
                trace_arg = f'OXRTL_ARGS_{param.type}((*{param.name}), "{param.name}")'
            if param.is_const:
                trace_in.append(trace_arg)
                if is_struct:
                    trace_next_in.append(self.genTraceStruct(xr_command.name, param))
                continue
            trace_out.append(trace_arg)
            if is_struct:
                trace_next_out.append(self.genTraceStruct(xr_command.name, param))

        instance_state_pre = ""
        instance_state_post = ""
        if xr_command.params[0].type == "XrInstance":
            if xr_command.name == "xrDestroyInstance":
                instance_state_post = f"""
if (gXrInstance == {xr_command.params[0].name}) {{
  gXrInstance = {{}};
}}
"""
            else:
                instance_state_pre = f"gXrInstance = {xr_command.params[0].name};"
        hook = ""
        if xr_command.name in hooked:
            hook = f"{xr_command.name}_hook(ret, " + ", ".join(arguments) + ");\n"
        return f"""
XrResult XRAPI_CALL OXRTracing_{xr_command.name}({', '.join(parameters)}) {{
  {instance_state_pre}
  TraceLoggingActivity<gTraceProvider> localActivity;
  TraceLoggingWriteStart(localActivity, {', '.join(trace_in)});
  {newline.join(trace_next_in)}
  try {{
    const auto ret = next_{xr_command.name}({', '.join(arguments)});
    {newline.join(trace_next_out)}
    {hook}
    TraceLoggingWriteStop(localActivity, {', '.join(trace_out)});
    {instance_state_post}
    return ret;
  }} catch (const std::exception& e) {{
      TraceLoggingWriteStop(localActivity, "{xr_command.name}", TraceLoggingValue(e.what(), "exception"));
      throw;
  }} catch(...) {{
      TraceLoggingWriteStop(localActivity, "{xr_command.name}", TraceLoggingValue("Got a C++ exception that is not an std exception", "exception"));
      throw;
  }}
}}
"""

    def beginFile(self, genOpts):
        BoilerplateOutputGenerator.beginFile(self, genOpts)
        content = """
#include <OXRTracing.hpp>

#include <TraceLoggingActivity.h>
#include <TraceLoggingProvider.h>

using namespace OXRTracing;
"""
        write(content, file=self.outFile)

    def endFile(self):
        content = f"""
{self.genNextPFNDefinitions()}

{self.genWrappers()}

{self.genXrGetInstanceProcAddr()}
"""
        write(content, file=self.outFile)
        BoilerplateOutputGenerator.endFile(self)


class ForwardDeclarationsOutputGenerator(BoilerplateOutputGenerator):
    def beginFile(self, genOpts):
        BoilerplateOutputGenerator.beginFile(self, genOpts)
        content = f"""
#include <OXRTracing/forward_declarations.hpp>
#include <OXRTracing/preamble.hpp>

#include <TraceLoggingActivity.h>

namespace OXRTracing {{
"""
        write(content, file=self.outFile)

    def genNextPFNDeclarations(self):
        ret = ""
        for xr_command in self.getWrappedCommands():
            ret += f"extern PFN_{xr_command.name} next_{xr_command.name};" + "\n"
        return ret

    def genDumpingFunctionDeclarations(self):
        ret = ""
        for xr_struct in sorted(
            self.api_structures, key=lambda xr_struct: xr_struct.name
        ):
            decl = self.getNextDumperDecl(xr_struct)
            if decl:
                ret += decl + ";\n"
        return ret

    def genTracingFunctionDeclarations(self):
        ret = ""
        for xr_command in self.getWrappedCommands():
            parameters = []
            for param in xr_command.params:
                parameters.append(param.cdecl.strip())
            ret += (
                f'XrResult XRAPI_CALL OXRTracing_{xr_command.name}({", ".join(parameters)});'
                + "\n"
            )
        return ret

    def endFile(self):
        content = f"""
{self.genNextPFNDeclarations()}
}}

{self.genDumpingFunctionDeclarations()}

{self.genTracingFunctionDeclarations()}
"""
        write(content, file=self.outFile)
        BoilerplateOutputGenerator.endFile(self)


def generate(gen, gen_opts):
    registry = Registry(gen, gen_opts)
    registry.loadFile(os.path.join(sdk_dir, "specification", "registry", "xr.xml"))
    registry.apiGen()


if __name__ == "__main__":
    conventions = OpenXRConventions()
    featuresPat = "XR_VERSION_1_0"
    extensionsPat = ".*"
    excludeExtensionsPat = (
        "^("
        + (
            "|".join(
                [
                    # Skip other platform APIs
                    ".+android.+",
                    "XR_KHR_loader.+",
                    "XR_ML_.+",  # Magic Leap
                    # Skip all graphics APIs
                    #
                    # These could be handled, just doesn't seem worth dealing with the various types
                    # defined in external SDKs and headers.
                    ".+opengl.*",
                    ".+_egl_.+",
                    # Skip debugging/profiling extensions
                    "XR_EXT_debug_utils",
                    "XR_KHR_convert_timespec_time",
                    "XR_KHR_win32_convert_performance_counter_time",
                ]
            )
        )
        + ")$"
    )

    gen = MacroOutputGenerator(diagFile=None)
    out_dir = os.path.join(cur_dir, "gen", "include", "OXRTracing")
    gen_opts = AutomaticSourceGeneratorOptions(
        conventions=conventions,
        filename="macros.gen.hpp",
        directory=out_dir,
        apiname="openxr",
        profile=None,
        versions=featuresPat,
        emitversions=featuresPat,
        defaultExtensions="openxr",
        addExtensions=extensionsPat,
        removeExtensions=excludeExtensionsPat,
        emitExtensions=extensionsPat,
    )
    generate(gen, gen_opts)

    # I added support for splitting into multiple files to try and stop
    # cl.exe failing with 'fatal error C1060: compiler is out of heap space'
    #
    # Didn't work for now, but might help in the future, so keeping the code in.
    #
    # It just hates xrEndFrame
    file_count = 1
    for file_number in range(0, file_count):
        gen = LayerOutputGenerator(file_number, file_count, diagFile=None)
        out_dir = os.path.join(cur_dir, "gen", "src")
        gen_opts = AutomaticSourceGeneratorOptions(
            conventions=conventions,
            filename=f"layer.gen.{file_number}.cpp",
            directory=out_dir,
            apiname="openxr",
            profile=None,
            versions=featuresPat,
            emitversions=featuresPat,
            defaultExtensions="openxr",
            addExtensions=extensionsPat,
            removeExtensions=excludeExtensionsPat,
            emitExtensions=extensionsPat,
        )
        generate(gen, gen_opts)

    gen = ForwardDeclarationsOutputGenerator(diagFile=None)
    out_dir = os.path.join(cur_dir, "gen", "include", "OXRTracing")
    gen_opts = AutomaticSourceGeneratorOptions(
        conventions=conventions,
        filename="forward_declarations.gen.hpp",
        directory=out_dir,
        apiname="openxr",
        profile=None,
        versions=featuresPat,
        emitversions=featuresPat,
        defaultExtensions="openxr",
        addExtensions=extensionsPat,
        removeExtensions=excludeExtensionsPat,
        emitExtensions=extensionsPat,
    )
    generate(gen, gen_opts)
