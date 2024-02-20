@echo off

REM  todo: not tested

echo "=========================================="
echo "=                                        ="
echo "=        Self-Signed Certificate         ="
echo "=               Generator                ="
echo "=                                        ="
echo "=========================================="
echo ""
read "-p" "Generate server certs [Y/N]:  " "GenerateServerCert"
REM UNKNOWN: {"type":"While","clause":{"type":"CompoundList","commands":[{"type":"LogicalExpression","op":"and","left":{"type":"Command","name":{"text":"[[","type":"Word"},"suffix":[{"text":"\"$GenerateServerCert\"","expansion":[{"loc":{"start":1,"end":19},"parameter":"GenerateServerCert","type":"ParameterExpansion"}],"type":"Word"},{"text":"!=","type":"Word"},{"text":"Y","type":"Word"}]},"right":{"type":"Command","name":{"text":"\"$GenerateServerCert\"","expansion":[{"loc":{"start":1,"end":19},"parameter":"GenerateServerCert","type":"ParameterExpansion"}],"type":"Word"},"suffix":[{"text":"!=","type":"Word"},{"text":"N","type":"Word"},{"text":"]]","type":"Word"}]}}]},"do":{"type":"CompoundList","commands":[{"type":"Command","name":{"text":"echo","type":"Word"},"suffix":[{"text":"-e","type":"Word"},{"text":"\nInvalid choice please choose Y or N\n","type":"Word"}]},{"type":"Command","name":{"text":"read","type":"Word"},"suffix":[{"text":"-p","type":"Word"},{"text":"Generate server certs [Y/N]:  ","type":"Word"},{"text":"GenerateServerCert","type":"Word"}]}]}}
read "-p" "Generate client cert [Y/N]:  " "GenerateClientCert"
REM UNKNOWN: {"type":"While","clause":{"type":"CompoundList","commands":[{"type":"LogicalExpression","op":"and","left":{"type":"Command","name":{"text":"[[","type":"Word"},"suffix":[{"text":"\"$GenerateClientCert\"","expansion":[{"loc":{"start":1,"end":19},"parameter":"GenerateClientCert","type":"ParameterExpansion"}],"type":"Word"},{"text":"!=","type":"Word"},{"text":"Y","type":"Word"}]},"right":{"type":"Command","name":{"text":"\"$GenerateClientCert\"","expansion":[{"loc":{"start":1,"end":19},"parameter":"GenerateClientCert","type":"ParameterExpansion"}],"type":"Word"},"suffix":[{"text":"!=","type":"Word"},{"text":"N","type":"Word"},{"text":"]]","type":"Word"}]}}]},"do":{"type":"CompoundList","commands":[{"type":"Command","name":{"text":"echo","type":"Word"},"suffix":[{"text":"-e","type":"Word"},{"text":"\nInvalid choice please choose Y or N\n","type":"Word"}]},{"type":"Command","name":{"text":"read","type":"Word"},"suffix":[{"text":"-p","type":"Word"},{"text":"Generate client cert [Y/N]:  ","type":"Word"},{"text":"GenerateClientCert","type":"Word"}]}]}}
read "-p" "Enter server IP/Domain: " "IP"
SET "SUBJECT_CA="/C"
SET "%IP%SUBJECT_SERVER="/C"
SET "%IP%%IP%SUBJECT_CLIENT="/C"
IF [[ "%GenerateServerCert%" == "Y" "]]" (
  CALL :generate_CA "%SUBJECT_CA%"
  CALL :generate_server "%SUBJECT_SERVER%"
)
IF [[ "%GenerateClientCert%" == "Y" "]]" (
  CALL :generate_client "%SUBJECT_CLIENT%"
)

EXIT /B %ERRORLEVEL%

:generate_CA
echo "-e" "
===== Generating CA certs ====="
openssl "req" "-x509" "-nodes" "-sha256" "-newkey" "rsa:2048" "-subj" "%~1" "-days" "365" "-keyout" "ca.key" "-out" "ca.crt"
echo "==================================================="
EXIT /B 0

:generate_server
echo "-e" "
===== Generating server certs ====="
openssl "req" "-nodes" "-sha256" "-new" "-subj" "%~1" "-keyout" "server.key" "-out" "server.csr"
openssl "x509" "-req" "-sha256" "-in" "server.csr" "-CA" "ca.crt" "-CAkey" "ca.key" "-CAcreateserial" "-out" "server.crt" "-days" "365"
echo "==================================================="
EXIT /B 0

:generate_client
echo "-e" "
===== Generating client certs ====="
openssl "req" "-new" "-nodes" "-sha256" "-subj" "%~1" "-out" "client.csr" "-keyout" "client.key"
openssl "x509" "-req" "-sha256" "-in" "client.csr" "-CA" "ca.crt" "-CAkey" "ca.key" "-CAcreateserial" "-out" "client.crt" "-days" "365"
echo "==================================================="
EXIT /B 0