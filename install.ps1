param (
	[Parameter(Mandatory)]
	[ValidateSet('First', 'Last', 'Enable', 'Disable', 'Remove')]
	[string] $Mode,
	[Parameter(Mandatory = $False)]
	[string] $LayerPath
)

$key = "HKLM:\SOFTWARE\Khronos\OpenXR\1\ApiLayers\Implicit"
if (-not (Test-Path $key)) {
	New-Item -ItemPath $key -Frce
}

$layers = Get-Item $key

if ("${LayerPath}" -eq "") {
	$LayerPath = (Get-Item out/APILayer.json).FullName
}

$have_this_layer = $null -ne $layers.GetValue($LayerPath, $null)

switch ($Mode) {
	"Disable" {
		if (-Not $have_this_layer) {
			Write-Host "Layer already not in registry"
			return;
		}
		Set-ItemProperty -Path $key -Name $LayerPath -Value 1
		Write-Host "Enabled layer ${LayerPath}."
		return;
	}
	"Enable" {
		if (-Not $have_this_layer) {
			Write-Host "Layer not in registry; use 'First' or 'Last' to install"
			return;
		}
		Set-ItemProperty -Path $key -Name $LayerPath -Value 0
		Write-Host "Enabled layer ${LayerPath}."
		return;
	}
	"Remove" {
		if (-Not $have_this_layer) {
			Write-Host "Layer already not in registry"
			return;
		}
		Remove-ItemProperty -Path $key -Name $LayerPath
		Write-Host "Removed layer ${LayerPath}"
		return;
	}
}

# Okay, if we get here, we're definitely rebuilding the layer list anyway. Mode is 'First' or 'Last'
$new_layers = @()
$this_layer = @{ Path = $LayerPath; Disabled = 0 }

if ($Mode -eq "First") {
	$new_layers += $this_layer
}

foreach ($layer in $layers.Property) {
	$disabled = $layers.GetValue($layer, $null)
	if ($null -eq $disabled) {
		continue;
	}
	if ($layer -eq $this_layer.Path) {
		continue;
	}

	$new_layers += @(@{ Path = $layer; Disabled = $disabled })
}

if ($Mode -eq "Last") {
	$new_layers += $this_layer
}

Clear-Item -Path $key
foreach ($layer in $new_layers) {
	$path = $layer.Path
	$disabled = $layer.Disabled
	Set-ItemProperty -Path $key -Name $path -Value $disabled
}

Write-Host "Updated OpenXR layer list."
