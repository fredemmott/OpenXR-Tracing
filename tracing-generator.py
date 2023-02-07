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
sdk_dir = os.path.join(base_dir, 'third-party', 'OpenXR-SDK-Source')  # nopep8
sys.path.append(os.path.join(sdk_dir, 'specification', 'scripts'))  # nopep8
sys.path.append(os.path.join(sdk_dir, 'src', 'scripts'))  # nopep8

from xrconventions import OpenXRConventions
from generator import write
from reg import Registry
from automatic_source_generator import AutomaticSourceOutputGenerator, AutomaticSourceGeneratorOptions


class BoilerplateOutputGenerator(AutomaticSourceOutputGenerator):
    '''Common generator utilities and formatting.'''

    def outputGeneratedHeaderWarning(self):
        warning = '''// *********** THIS FILE IS GENERATED - DO NOT EDIT ***********'''
        write(warning, file=self.outFile)

    def outputCopywriteHeader(self):
        copyright = '''// MIT License
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
'''
        write(copyright, file=self.outFile)

    def isEmptyStruct(self, name):
        if self.getRelationGroupForBaseStruct(name) is not None:
            return False
        for xr_struct in self.api_structures:
            if xr_struct.name != name:
                continue
            # 'next' and 'type'
            return len(xr_struct.members) == 2
        return False

    def getWrappedCommands(self):
        handwritten = ["xrGetInstanceProcAddr", 'xrCreateInstance']
        skip = handwritten + self.no_trampoline_or_terminator
        all = self.core_commands + self.ext_commands
        return [command for command in all if command.name not in skip]

    def outputGeneratedAuthorNote(self):
        pass


class MacroOutputGenerator(BoilerplateOutputGenerator):
    def genBaseTypeMacros(self):
        handwritten = {'XrAction', 'XrActionSet'}

        ret = ''
        for xr_type in self.api_base_types:
            ret += self.genBaseTypeMacro(xr_type) + "\n"
        for xr_type in self.api_handles:
            macro = f'#define OXRTL_ARGS_{xr_type.name}(oxrtlIt, name) OXRTL_ARGS_HANDLE(oxrtlIt, name)' + '\n'
            if xr_type.name in handwritten:
                ret += '// EXCLUDED - HANDWRITTEN:\n// '+macro
            else:
                ret += macro
        for xr_type in self.api_flags:
            ret += f'#define OXRTL_ARGS_{xr_type.name}(oxrtlIt, name) OXRTL_ARGS_{xr_type.type}(oxrtlIt, name)' + '\n'

        for xr_type in (self.api_base_types + self.api_handles):
            ret += f'#define OXRTL_ARGS_{xr_type.name}_DA(oxrtlIt, name, size) TraceLoggingValue(size, "#" name)' + '\n'

        for xr_type in (self.api_base_types + self.api_handles):
            ret += f'''
#define OXRTL_DUMP_{xr_type.name}(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt)
    TraceLoggingWriteTagged(
        oxrtlActivity,
        oxrtlName,
        OXRTL_ARGS_{xr_type.name}(oxrtlIt, oxrtlValueName));
'''.strip().replace('\n', '\\\n') + '\n'
        return ret

    def genBaseTypeMacro(self, xr_type):
        handwritten_types = {'XrVersion', 'XrPath'}
        if xr_type.type == 'XR_DEFINE_ATOM':
            ret = f'#define OXRTL_ARGS_{xr_type.name}(oxrtlIt, name) OXRTL_ARGS_ATOM(oxrtlIt, name)'
        else:
            ret = f'#define OXRTL_ARGS_{xr_type.name}(oxrtlIt, name) OXRTL_ARGS_{xr_type.type}(oxrtlIt, name)'
        if xr_type.name in handwritten_types:
            return '// EXCLUDED - HANDWRITTEN:\n// ' + ret
        return ret

    def genEnumMacros(self):
        ret = ''
        for xr_enum in self.api_enums:
            ret += self.genEnumMacro(xr_enum) + "\n"
        return ret

    def genEnumMacro(self, xr_enum):
        newline = '\n'
        values = [value for value in xr_enum.values if value.alias is None]
        return f'''
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
#define OXRTL_ARGS_{xr_enum.name}(oxrtlIt, name) TraceLoggingValue(OXRTracing::to_string(oxrtlIt).c_str(), name)
'''

    def genStructMacros(self):
        ret = ''
        for xr_struct in self.api_structures:
            ret += self.genStructMacro(xr_struct) + "\n"
        return ret

    def genStructMacro(self, xr_struct):
        handwritten = {'XrEventDataBuffer'}
        member_macros = []
        for member in xr_struct.members:
            if member.name == 'next':
                continue
            if member.name == 'type' and member.type == "XrStructureType" and self.getRelationGroupForBaseStruct(xr_struct.name) is None:
                continue
            if self.isEmptyStruct(member.type):
                continue
            suffix = ''
            trailing = ''
            pointer_count = member.pointer_count
            if member.is_array:
                if member.is_static_array:
                    suffix += '_FA'
                    trailing += f', {member.static_array_sizes[0]}'
                elif member.type == 'char':
                    suffix += '_DA'
                    pointer_count -= 1
                    trailing += f', oxrtlIt.{member.pointer_count_var}'
                else:
                    continue
            if pointer_count > 0:
                suffix = '_' + ('P' * pointer_count) + suffix
            member_macros.append(
                f'OXRTL_ARGS_{member.type}{suffix}(oxrtlIt.{member.name}, "{member.name}"{trailing})')
        if xr_struct.name in handwritten:
            struct_def = f'// EXCLUDED - HANDWRITTEN: #define OXRTL_ARGS_{xr_struct.name}(oxrtlIt, name)'
        elif member_macros:
            struct_def = f'#define OXRTL_ARGS_{xr_struct.name}(oxrtlIt, name) TraceLoggingStruct({len(member_macros)}, name),{", ".join(member_macros)}'
        else:
            struct_def = f'#define OXRTL_ARGS_{xr_struct.name}(oxrtlIt, name) TraceLoggingValue(name)'
        return f'''
{struct_def}
#define OXRTL_ARGS_{xr_struct.name}_P(oxrtlIt, name) OXRTL_ARGS_{xr_struct.name}((*oxrtlIt), name)
#define OXRTL_ARGS_{xr_struct.name}_DA(oxrtlIt, name, size) TraceLoggingValue(size, "#" name)
#define OXRTL_ARGS_{xr_struct.name}_P_DA(oxrtlIt, name, size) TraceLoggingValue(size, "#" name)
{self.genDumpStructMacro(xr_struct)}
'''

    def genDumpComplexMember(self, xr_member):
        if xr_member.is_array and not xr_member.is_static_array:
            return self.genDumpDynamicArrayMember(xr_member)
        return None

    def genDumpDynamicArrayMember(self, xr_member):
        index_type = f'decltype(oxrtlIt.{xr_member.pointer_count_var})'
        i = f'{xr_member.type}_i'

        return f'''
{{
    for ({index_type} {i} = 0; {i} < oxrtlIt.{xr_member.pointer_count_var}; ++{i}) {{
        OXRTL_DUMP_{xr_member.type}(
            oxrtlActivity,
            oxrtlName "_{xr_member.name}",
            "element",
            ({'*' * (xr_member.pointer_count - 1)}oxrtlIt.{xr_member.name}[{i}]));
    }}
}}
'''.strip().replace('\n', '\\\n')

    def genChildStructCase(self, xr_struct):
        type_param = [it for it in xr_struct.members if it.name == "type"][0]
        xr_type_value = type_param.values
        return f'''
case {xr_type_value}:
    OXRTL_DUMP_{xr_struct.name}(
            oxrtlActivity,
            oxrtlName,
            "{xr_type_value}",
            (*reinterpret_cast<const {xr_struct.name}*>(&oxrtlIt)));
    break;
'''.strip()

    def genDumpStructMacro(self, xr_struct):
        complex_fields = []
        for member in xr_struct.members:
            member_dumper = self.genDumpComplexMember(member)
            if member_dumper is not None:
                complex_fields.append(member_dumper.strip())
        ret = f'#define OXRTL_DUMP_{xr_struct.name}_COMPLEX_FIELDS(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt)'
        if complex_fields:
            ret += "\\\n".join(complex_fields)
        ret += "\n"
        ret += f'#define OXRTL_DUMP_{xr_struct.name}(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt)'
        dump_base = f'''
    TraceLoggingWriteTagged(
        oxrtlActivity,
        oxrtlName,
        OXRTL_ARGS_{xr_struct.name}(oxrtlIt, oxrtlValueName));
    OXRTL_DUMP_{xr_struct.name}_COMPLEX_FIELDS(oxrtlActivity, oxrtlName, oxrtlValueName, oxrtlIt);
'''.strip()
        relations = self.getRelationGroupForBaseStruct(xr_struct.name)
        if relations is None:
            ret += dump_base.replace('\n', '\\\n')
            return ret
        descendants = []
        to_visit = relations.child_struct_names
        while to_visit:
            name = to_visit.pop()
            struct = self.getStruct(name)
            descendants.append(struct)
            relations = self.getRelationGroupForBaseStruct(name)
            if relations:
                to_visit += relations.child_struct_names
        ret += f'''
switch(oxrtlIt.type) {{
{''.join([
self.genChildStructCase(xr_struct)
for xr_struct in descendants
])}
  default:
    {dump_base}
}}'''.strip().replace('\n', '\\\n')
        return ret

    def beginFile(self, genOpts):
        BoilerplateOutputGenerator.beginFile(self, genOpts)
        write('''
#pragma once

#include <openxr/openxr.h>

#include <format>
''', file=self.outFile)

    def endFile(self):
        contents = f'''
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
'''
        write(contents, file=self.outFile)
        BoilerplateOutputGenerator.endFile(self)


class LayerOutputGenerator(BoilerplateOutputGenerator):
    def genNextPFNDefinitions(self):
        ret = 'namespace OXRTracing {\n'
        for xr_command in self.getWrappedCommands():
            ret += f'PFN_{xr_command.name} next_{xr_command.name} {{nullptr}};' + '\n'
        return ret + '\n}'

    def genXrGetInstanceProcAddr(self):
        ret = '''
XrResult OXRTracing_xrGetInstanceProcAddr(
    XrInstance instance,
    const char* nameCStr,
    PFN_xrVoidFunction* function) {

    *function = nullptr;
    const auto ret = gXrNextGetInstanceProcAddr(instance, nameCStr, function);
    if (XR_FAILED(ret) || !*function) {
        return ret;
    }
    const std::string_view name {nameCStr};
'''
        for xr_command in self.getWrappedCommands():
            ret += f'''
if (name == "{xr_command.name}") {{
    next_{xr_command.name} = reinterpret_cast<PFN_{xr_command.name}>(*function);
    *function = reinterpret_cast<PFN_xrVoidFunction>(&OXRTracing_{xr_command.name});
    return ret;
}}
'''
        ret += '''
    return ret;
}'''
        return ret

    def genWrappers(self):
        ret = ''
        for xr_command in self.getWrappedCommands():
            if xr_command.name in self.no_trampoline_or_terminator:
                continue
            ret += self.genWrapper(xr_command)
        return ret

    def genTraceStruct(self, command_name, xr_param, top_level=True):
        struct_name = xr_param.type
        return f'''
OXRTL_DUMP_{struct_name}(
    localActivity,
    "{command_name}",
    "{xr_param.name}",
    (*{xr_param.name})
);
'''.strip()

    def genWrapper(self, xr_command):
        hooked = {"xrCreateActionSet", "xrCreateAction", "xrStringToPath"}
        newline = "\n"

        parameters = []
        arguments = []
        trace_in = [f'"{xr_command.name}"']
        trace_out = [f'"{xr_command.name}"',
                     'OXRTL_ARGS_XrResult(ret, "XrResult")']
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
            if is_struct or param.type == "char":
                trace_arg = f'OXRTL_ARGS_{param.type}_P({param.name}, "{param.name}")'
            else:
                trace_arg = f'OXRTL_ARGS_{param.type}((*{param.name}), "{param.name}")'
            if param.is_const:
                trace_in.append(trace_arg)
                if is_struct:
                    trace_next_in.append(
                        self.genTraceStruct(xr_command.name, param))
                continue
            trace_out.append(trace_arg)
            if is_struct:
                trace_next_out.append(self.genTraceStruct(
                    xr_command.name, param))

        instance_state_pre = ''
        instance_state_post = ''
        if xr_command.params[0].type == "XrInstance":
            if xr_command.name == "xrDestroyInstance":
                instance_state_post = f'''
if (gXrInstance == {xr_command.params[0].name}) {{
  gXrInstance = {{}};
}}
'''
            else:
                instance_state_pre = f'gXrInstance = {xr_command.params[0].name};'
        hook = ''
        if xr_command.name in hooked:
            hook = f'{xr_command.name}_hook(ret, ' + \
                ', '.join(arguments) + ');\n'
        return f'''
XrResult OXRTracing_{xr_command.name}({', '.join(parameters)}) {{
  {instance_state_pre}
  TraceLoggingActivity<gTraceProvider> localActivity;
  TraceLoggingWriteStart(localActivity, {', '.join(trace_in)});
  {newline.join(trace_next_in)}
  const auto ret = next_{xr_command.name}({', '.join(arguments)});
  {newline.join(trace_next_out)}
  {hook}
  TraceLoggingWriteStop(localActivity, {', '.join(trace_out)});
  {instance_state_post}
  return ret;
}}
'''

    def beginFile(self, genOpts):
        BoilerplateOutputGenerator.beginFile(self, genOpts)
        content = '''
#include <openxr/openxr.h>
#include <OXRTracing.hpp>

#include <TraceLoggingActivity.h>
#include <TraceLoggingProvider.h>

using namespace OXRTracing;
'''
        write(content, file=self.outFile)

    def endFile(self):
        content = f'''
{self.genNextPFNDefinitions()}

{self.genWrappers()}

{self.genXrGetInstanceProcAddr()}
'''
        write(content, file=self.outFile)
        BoilerplateOutputGenerator.endFile(self)


class ForwardDeclarationsOutputGenerator(BoilerplateOutputGenerator):
    def beginFile(self, genOpts):
        BoilerplateOutputGenerator.beginFile(self, genOpts)
        content = f'''
#include <openxr/openxr.h>
#include <OXRTracing.hpp>

namespace OXRTracing {{
'''
        write(content, file=self.outFile)

    def genNextPFNDeclarations(self):
        ret = ''
        for xr_command in self.getWrappedCommands():
            ret += f'extern PFN_{xr_command.name} next_{xr_command.name};' + '\n'
        return ret

    def endFile(self):
        content = f'''
{self.genNextPFNDeclarations()}
}}
'''
        write(content, file=self.outFile)
        BoilerplateOutputGenerator.endFile(self)


def generate(gen, gen_opts):
    registry = Registry(gen, gen_opts)
    registry.loadFile(os.path.join(
        sdk_dir, 'specification', 'registry', 'xr.xml'))
    registry.apiGen()


if __name__ == '__main__':
    conventions = OpenXRConventions()
    featuresPat = 'XR_VERSION_1_0'
    # Extensions with TODOs currently generate invalidate C++ code
    #
    # Prefix patterns are just here for convenience, because the generator is
    # compatible with all their *current* extensions; future extensions might
    # break compatibility, or be unsuitable.
    extensionsPat = '^' + ('|'.join([
        'XR_EXT_active_action_set_priority',
        'XR_EXT_dpad_binding',
        # SKIPPED - let's not add noise: 'XR_ext_debug_utils',
        'XR_EXT_eye_gaze_interaction',
        'XR_EXT_hand_tracking',
        'XR_EXT_hp_mixed_reality_controller',
        'XR_EXT_palm_pose',
        'XR_EXT_samsung_odyssey_controller',
        'XR_EXT_performance_settings',
        'XR_EXT_thermal_query',
        'XR_EXT_view_configuration_depth_range',
        'XR_KHR_binding_modification',
        'XR_KHR_composition_layer.+',
        # TODO: 'XR_KHR_visibility_mask',
        'XR_AMALENCE_.+',
        'XR_EPIC_.+',
        # TODO: 'XR_HTC.+',
        # TODO: 'XR_FB.+',
        'XR_META_.+',
        'XR_ML_.+',
        'XR_MND_.+',
        # TODO: 'XR_MSFT_.+',
        # TODO: 'XR_OCULUS_.+',
        'XR_ULTRALEAP_.+',
        'XR_VALVE_.+',
        'XR_VARJO_.+',
        'XR_HTCX_.+',
        'XR_MNDX_.+',
    ])) + '$'

    gen = MacroOutputGenerator(diagFile=None)
    out_dir = os.path.join(cur_dir, "gen", "include", "OXRTracing")
    gen_opts = AutomaticSourceGeneratorOptions(
        conventions=conventions,
        filename='macros.gen.hpp',
        directory=out_dir,
        apiname='openxr',
        profile=None,
        versions=featuresPat,
        emitversions=featuresPat,
        defaultExtensions='openxr',
        addExtensions=None,
        removeExtensions=None,
        emitExtensions=extensionsPat)
    generate(gen, gen_opts)

    gen = LayerOutputGenerator(diagFile=None)
    out_dir = os.path.join(cur_dir, "gen", "src")
    gen_opts = AutomaticSourceGeneratorOptions(
        conventions=conventions,
        filename='layer.gen.cpp',
        directory=out_dir,
        apiname='openxr',
        profile=None,
        versions=featuresPat,
        emitversions=featuresPat,
        defaultExtensions='openxr',
        addExtensions=None,
        removeExtensions=None,
        emitExtensions=extensionsPat)
    generate(gen, gen_opts)

    gen = ForwardDeclarationsOutputGenerator(diagFile=None)
    out_dir = os.path.join(cur_dir, "gen", "include", "OXRTracing")
    gen_opts = AutomaticSourceGeneratorOptions(
        conventions=conventions,
        filename='forward_declarations.gen.hpp',
        directory=out_dir,
        apiname='openxr',
        profile=None,
        versions=featuresPat,
        emitversions=featuresPat,
        defaultExtensions='openxr',
        addExtensions=None,
        removeExtensions=None,
        emitExtensions=extensionsPat)
    generate(gen, gen_opts)
