<#
Ejecuta ejemplos rápidos de cada binario (asumiendo que ya se compiló con build_all.ps1)
Uso:
  powershell -ExecutionPolicy Bypass -File .\run_examples.ps1
#>

$ErrorActionPreference = 'Stop'

function RunCmd($cmd, $args) {
  Write-Host "\n> $cmd $args" -ForegroundColor Yellow
  & .\$cmd $args
}

RunCmd "mv_a.exe" ""
RunCmd "mv_b.exe" "800 800"
RunCmd "mv_c.exe" "800 800 8"
RunCmd "hp_a.exe" ""
RunCmd "hp_b.exe" "256 500"
RunCmd "hp_c.exe" "256 500 8"
RunCmd "mo_a.exe" ""
RunCmd "mo_b.exe" "120 120 120"
RunCmd "mo_c.exe" "120 120 120"
RunCmd "component_labeling.exe" "256 256 0.4 4"
RunCmd "game_of_life.exe" "256 256 100 0.3"

Write-Host "\nEjecuciones ejemplo completadas" -ForegroundColor Green
