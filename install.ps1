param (
	[Parameter(Mandatory)]
	[ValidateSet('First', 'Last', 'Enable', 'Disable', 'Remove', 'Before', 'After')]
	[string] $Mode,
	[Parameter(Mandatory = $False)]
	[string] $LayerPath,
	[Parameter(Mandatory = $False)]
	[string] $RelativeTo
)

if ("${LayerPath}" -eq "") {
	$LayerPath = (Get-Item out/APILayer.json).FullName
}

if (-not (Test-Path $LayerPath)) {
	Write-Host "LayerPath '${LayerPath}' does not exist."
	return 1
}

if ($Mode -eq "Before" -or $Mode -eq "After") {
	if ("${RelativeTo}" -eq "") {
		Write-Host "Specify -RelativeTo if specifying -Mode Before or -Mode After."
		return 1
	}
}


$key = "HKLM:\SOFTWARE\Khronos\OpenXR\1\ApiLayers\Implicit"
$layers = Get-Item $key

if (-not (Test-Path $key)) {
	New-Item -ItemPath $key -Frce
}

$have_this_layer = $null -ne $layers.GetValue($LayerPath, $null)

if ("${RelativeTo}" -ne "") {
	$have_relative_layer = $null -ne $layers.GetValue($RelativeTo, $null)
	if (-not $have_relative_layer) {
		Write-Host "-RelativeTo '${RelativeTo}' is not in the registry."
		return 1
	}
}

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

	$relative = $RelativeTo -eq $layer
	if ($relative -and $Mode -eq "Before") {
		$new_layers += $this_layer
	}

	$new_layers += @(@{ Path = $layer; Disabled = $disabled })

	if ($relative -and $Mode -eq "After") {
		$new_layers += $this_layer
	}
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
