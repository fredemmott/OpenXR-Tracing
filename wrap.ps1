param (
	[Parameter(Mandatory = $True)]
	[string] $RelativeTo
)

if (Test-Path out/APILayer.json) {
	$LayerPath = "out/APILayer.json"
} elseif (Test-Path APILayer.json) {
	$LayerPath = "APILayer.json"
}

if ("${LayerPath}" -eq "") {
	Write-Host "Could not find APILayer.json"
	return 1
}

$LayerPath = (Get-Item $LayerPath).FullName
$AltLayerPath = Join-Path (Get-Item $LayerPath).Directory.FullName "APILayer_Alternate.json"

$Install = Join-Path (Get-Item $PSCommandPath).Directory.FullName "install.ps1"

& $Install -Mode Before -LayerPath $LayerPath -RelativeTo $RelativeTo
& $Install -Mode After -LayerPath $AltLayerPath -RelativeTo $RelativeTo
