param (
	[Parameter(Mandatory = $True)]
	[string] $RelativeTo
)

.\install.ps1 -Mode Before -LayerPath (Get-Item out\APILayer.json).FullName -RelativeTo $RelativeTo
.\install.ps1 -Mode After -LayerPath (Get-Item out\APILayer_Alternate.json).FullName -RelativeTo $RelativeTo
