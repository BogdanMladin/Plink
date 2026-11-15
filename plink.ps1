param(
    [string]$p1,
    [string]$p2,
    [switch]$add,
    [switch]$open
)
$recordpath = $PSScriptRoot + "\record.txt"
if($add){
    $sumatra = (Get-Process SumatraPDF | Select-Object -First 1).Path
    & $sumatra -dde '[CmdCopyLocationToClipboard]'
    Start-Sleep -Milliseconds 200
    if($p1){
        $p1 | Out-File $recordpath -a -e ascii -NoNewline
        " : " | Out-File $recordpath -a -e ascii -NoNewline
        Get-Clipboard | Out-File $recordpath -Append
    }
    else{
        echo "no link provided"
    }
}
if($open){
    # $entries = Get-Content .\record.txt
    # foreach($entry in $entries){
    #     $code = $entry | Select-String -Pattern '\w'
    #     echo $code
    # }
    
    $pattern = $p1 + ' :'
    $entry = Select-String -Path $recordpath -Pattern $p1    

    if ($entry -match '-page\s+(\d+).*"([^"]+\.pdf)"') {
        $page = [int]$Matches[1] + 1
        $path = $Matches[2]
        echo $page
        echo $path
    }

    sumatrapdf -page $page $path
    # $sumatra = (Get-Process SumatraPDF | Select-Object -First 1).Path

    # & $sumatra -page $page $pdfPath
}


# $sumatra = (Get-Process SumatraPDF | Select-Object -First 1).Path
# & $sumatra -dde '[CmdCopyLocationToClipboard]'
# Start-Sleep -Milliseconds 200
# Get-Clipboard

# New-Item -ItemType Directory -Force -Path ./files
