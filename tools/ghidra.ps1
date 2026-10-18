# Launch the Ghidra GUI (or, with -Headless, the headless analyzer) bundled in this tools folder,
# using the bundled Temurin JDK 25. Settings/cache/temp are kept under ..\ghidra-home so they do not
# land in AppData (when launched from the Claude desktop app, AppData writes are MSIX-virtualized and
# Ghidra's OSGi bundle cache refuses the redirected path).
#
#   .\tools\ghidra.ps1                      # GUI; open project C:\rush2049-dreamcast\ghidra-project\RushDC.gpr
#   .\tools\ghidra.ps1 -Headless <args...>  # analyzeHeadless with the same environment
param([switch]$Headless, [Parameter(ValueFromRemainingArguments = $true)][string[]]$Rest)
$tools = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Split-Path -Parent $tools
$env:JAVA_HOME = Join-Path $tools "jdk\jdk-25.0.4.1+1"
$env:PATH = "$env:JAVA_HOME\bin;$env:PATH"
$home_ = Join-Path $root "ghidra-home"
foreach ($d in "settings", "cache", "tmp") { New-Item -ItemType Directory -Force (Join-Path $home_ $d) | Out-Null }
$env:JAVA_TOOL_OPTIONS = "-Dapplication.settingsdir=$home_\settings -Dapplication.cachedir=$home_\cache -Dapplication.tempdir=$home_\tmp"
$ghidra = Join-Path $tools "ghidra\ghidra_12.1.4_PUBLIC"
if ($Headless) {
    & (Join-Path $ghidra "support\analyzeHeadless.bat") @Rest
} else {
    & (Join-Path $ghidra "ghidraRun.bat") @Rest
}
