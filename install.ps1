param (
  [Parameter(Mandatory)]
  [ValidateSet('First','Last','Enable','Disable','Remove')]
  [string] $Mode
)

$key = "HKLM:\SOFTWARE\Khronos\OpenXR\1\ApiLayers\Implicit"
if (-not (Test-Path $key)) {
	New-Item -ItemPath $key -Frce
}

$layers = Get-Item $key

$this_layer = (Get-Item out/APILayer.json).FullName
$have_this_layer = $layers.GetValue($this_layer, $null) -ne $null

switch ($Mode) {
	"Disable" {
		if (-Not $have_this_layer) {
			Write-Host "Layer already not in registry"
			return;
		}
		Set-ItemProperty -Path $key -Name $this_layer -Value 1
		Write-Host "Enabled layer ${this_layer}."
		return;
	}
	"Enable" {
		if (-Not $have_this_layer) {
			Write-Host "Layer not in registry; use 'First' or 'Last' to install"
			return;
		}
		Set-ItemProperty -Path $key -Name $this_layer -Value 0
		Write-Host "Enabled layer ${this_layer}."
		return;
	}
	"Remove" {
		if (-Not $have_this_layer) {
			Write-Host "Layer already not in registry"
			return;
		}
		Remove-ItemProperty -Path $key -Name $this_layer
		Write-Host "Removed layer ${this_layer}"
		return;
	}
}

# Okay, if we get here, we're definitely rebuilding the layer list anyway. Mode is 'First' or 'Last'
$new_layers = @()
$this_layer = @{ Path = $this_layer; Disabled = 0 }

if ($Mode -eq "First") {
	$new_layers += @($this_layer)
}

foreach ($layer in $layers.Property) {
	$disabled = $layers.GetValue($layer, $null)
	if ($disabled -eq $null) {
		continue;
	}
	if ($layer -eq $this_layer.Path) {
		continue;
	}

	$new_layers += @(@{ Path = $layer; Disabled = $disabled})
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
