const char* ssid = "xxxxxx";
const char* password = "xxxxxx";

const char* mqtt_server = "domotich.es";      // MQTT Broker IP/URL
const int mqtt_port = 8883;                   // MQTT Broker Port
const char* uniqueID = "esp-client-secure-01";    // Unique Client Name
const int mqtt_buffer_size = 100000;

const char* mqttTopicStateIn_1 = "ham/domotichdeck/in";
const char* mqttTopicStateIn_2 = "ham/domotichdeck/in/+";
const char* mqttTopicFinish = "ham/domotichdeck/finish";


const char* FINGERPRINT = "24:22:99:E7:91:EE:AA:EB:36:CF:74:D4:70:25:B6:08:6C:44:BA:AE";

static const char CA_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDozCCAougAwIBAgIUXxPQqHgsf12owmm/ef4IRakHW6owDQYJKoZIhvcNAQEL
BQAwasdasdasdsUEBhMCRVMxDjAMBgNVBAgMBVNwYWluMQ4wDAYDVQQHDAVTcGFp
bjEPMA0GA1UECgwGTXlIb21lMQswCQYDVQQLDAJDQTEUMBIGA1UEAwwLZG9tb3Rp
Y2guZXMwHhcNMjMxMDIzMTYxNTyuiuiuiuiuMDIyMTYxNTQ4WjBhMQswCQYDVQQG
EwJFUzEOMAwGA1UECAwFU3BhaW4xDjAMBgNVBAcMBVNwYWluMQ8wDQYDVQQKDAZN
eUhvbWUxCzAJBgNVBAsMAkNBMRQwEgYDVQQDDAtkb21vdGljaC5lczCCASIwDQYJ
KoZIhvcNAQEBBQADggEPADCCAQoCggEBAL30lBArDNLYmh0TAYBp6ox9jBFSbHLh
GpXDu0sfbVCZ+QXRclgqP4TwNS01asdasd9Nme4AhDZH39EtoHn6JJPV3ZIDMjBj
87tCipMHlKrCCm000qkXXCIMgKkqlgNjXSonyil47yLDYT2DISTU8U2iYkC4skKa
lxrBvAvpmg08NO7nbbKBKw1qaOIzwwFbndsAwdZjRZsIlrEP6KBJoKX74Baa1e4f
XO28vKTnjVuEfj8Z8Qaf7Xus23S1J3F+4E8Qbx+FlOm979MnV20IFL0WXEvrXOiA
AYqPPVPguN0QT6v5bRpe1OV4lLotOBWgt96P6I0/fLz2s+C4/K2w4j8CAwEAAaNT
MFEwHQYDVR0OBBYEFKi8CIFbu0QdhwHfAWfMqdMB0lC2MB8GA1UdIwQYMBaAFKi8
CIFbu0QdhwHfAWfMqdMB0lC2MA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEL
BQADggEBAEWouasdasdsdSzLyH9YLxjbf9+wDvPLD1OHtCVVCkdndnuZcZY4bR0c
6zs0yBsQbqTYOFKCkFuz/Z+lw+MrNYHfrV2A2ciHk2pcWcVOHGNCqXyMMpd5VwWw
8BTKur3XO+bOW71vz8aVw93hThgZYdyozKsh4asdasdwerranKsEnvsBlTcROa6a
+TlJlsrvJ+xvwV52B+Py1/kkOAhOpBdWDO5g+mc9oTVgop+Ku2MJZVPYD8Q5Wa1u
0F48xDoFsd456jkfgsdu411cLfV1rb6OuTdRpgJ3l2ZyPxdasdykdRQC8E5AUuI+
QI9iuaaD3ScWBLdY7GJCcLjOEONBljU=
-----END CERTIFICATE-----
)EOF";


static const char CLIENT_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDTTCCas3456fgfg7fgh6y5sedfghtfKsv3b+2MA0GCSqGSIb3DQEBCwUAMGEx
CzAJBgNVBAYTAkVTMQ4wDAYDVQQIDAVTcGFpbjEOMAwGA1UEBwwFU3BhaW4xDzAN
BgNVBAoMBk15SG9tZTELMAkGA1UECwwCQ0ExFDASBgNVBAMMC2RvbW90aWNoLmVz
MB4XDTIzMTAyMzE2MTU1N1oXDTI0MTAyMjE2MTU1N1owZTELMAkGA1UEBhMCRVMx
DjAMBgNVBAgMBVNwYWluMQ4wDAYDVQQHDAVTcGFpbjEPMA0GA1UECgwGTXlIb21l
MQ8wDQYDVQQLDAZDsdfgfnQxFDASBgNVBAMMC2RvbW90aWNoLmVzMIIBIjANBgkq
hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA3ejS5uEbn7k4x3cNx6viQdHLqMy4A0dj
JAHsYtyDObPKIV3VZQNG5TIKUXYTJ4zqU8nNNXDg8RMTpI4sJH9kJOdJ//q+XBsy
El6vVylIg2VX/oZGgiLqTKNlM9SQT7QI2+0F9hxGc74osAFLvxbJJ4I3kqNrKu2S
WVyATwfZGXorZZCYwBj82iJ/GasdfsddfserACu48A7kBW80IX7QVUy6ZNHLi9IR
utJyO6iTg4ikfwREcXvi0LBsgjFO0wB0/jwnEfnO26BUP7Vqu6uZQx36d+ROOYAZ
nzGqrmynvISAWKL6oXOTO9md6/Voys/5lFCUftKKBpB5AzYniNQlgwIDAQABMA0G
CSqGSIb3DQEBCwUAA4IBAQAxafird7ndpNe0HWUiydyi5DpjOM6OaEyUJUsMJF7n
kShAq/k1LABsdftyhjhsdfsdfrg0mp8m0e6yplRo7FeLjUrhvzgnfHV910hFwE/4
d+M+ulSPetdJotTW7YzEurgBrhE5xEMApQNA8euqlaYsZZCQAzeFpCpkLp/Je9K7
Dv0XUfahpYvB9mV1FlAx7TI3fgrIDjlxEbf6ev8kprkXhevadnZ07329Kyp210GS
9RQO7u1SRhYcBeoqq3oB8wI2NcnNXLcNvV6gWUwRf8pHO56vYoeLx5FC51A74fr0
330O8XurrHIyABeGumSSlG2fMMQgpF54qsVbe9VJ3JFB
-----END CERTIFICATE-----
)EOF";

static const char CLIENT_KEY[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDd6NLm4RufuTjH
dw3Hq+JB0cuozLgDR2MkAexi3IM5s8ohXdVlA0blMgpRdhMnjOpTyc01cODxExOk
jiwkf2Qk50n/+wer57dfukdXKUiDZVf+hkaCIupMo2Uz1JBPtAjb7QX2HEZzviiw
AUu/FskngjeSo2sq7ZJZXIBPB9kZeitlkJjAGPzaIn8b/qL4JD5z3TYAK7jwDuQF
bzQhftBVTLpk0cuL0hG60nI7qJODiKR/BERxe+LQsGyCMU7TAHT+PCcR+c7boFQ/
tWq7q5lDHfp35E45gBmfMaqubKe8hIBYovqhc5M72Z3r9WjKz/mUUJR+0ooGkHkD
NieI1CWDAgMBAAECggEsdf43gdcgsddfas6gfVNkCwI4g3EtkzssaWMINz1Vi+y
RvdyrdoRQJGkkw4ovFiVohvLUKwNti7EjA/DzS3j7KlQEfVaNyYkRZZJhagUTMwY
AK0sYDZp49JVPV005LaffbNYHHbAriehy0wSJWi61UEE8B61BVUCCP0y79KXC9gW
UllKdi3+3GS6d3lNZq7Af1Dav3Dyc0LrmQfl/Pk4602odDp4mCDhWxE+eY0/sKI6
NXna+Wdhy79dsfg674dcgjyufMnOQDB+e37Ift/qluGDoGQ3v4wKY5/gKmVpRzDp
zSBVXBWxxK2IHq3Z8cvoMg0zHOSky7On0jm0Xwk/R4ECgYEA9D3PAfQGx00SNdPL
eoxI8JfDWeHFLAptO9u7FNXtF2Uc4Z1Go36MLFW/+fGX8cht2IywC+hQdLtjC6hX
Bk5gTgtKePmf7bM1WSnnbtkYA25jfPtFhbA6x7zF7wsMzlriVVuRhWoH8A8gMkQX
V1TyVDBDj1bR+UcsHPGo52qXg8kCgYEA6JfICBc3i/Pj9V+zOO14kn/1icymJvLy
Qsh4HvS69IjfrwjQ/e2f5u0oDKPp60MYm9dzlhIaiop2jpSGp3W4alL6IRqLtlGu
noorlkOi9g+o68dG9+mDOo9//AFf3gyUvax4/BcwxAVfld5CGav92wawtNU4rMeH
tBzZqhXqzOsCgYBx/aLuu3CqdesI2kd+c5jbSUlwWrHGgztGE6B18f4MzDE1j0zX
o4lSa9cElWAgBTZXOho0BIIAaTCp0TCEjfHemuBwI7kjnF9KtQcBsirF9zBRtYhW
A9D7Rbxve43HhZ1azzjUto7r2HNjYmouZUQQTNJLRO8M083f2F3HW/VJGQKBgExC
D0xPyF9zXCcD9G/Ss/hcSbWfSk500ucAcRm9JVqtUaFCo7rNYOLGmJ3kU/pVakkW
D4iBcQBBO3Ot0KQw3YDGqcmWuX3SNA04weenT1ObeMes82Cux964U77c+ICF4XJ6
jMQiGlP6awdo8oUWNcnM3Ly9N7K2TJDJ8LdqLix3AoGAEbkWRcSqf1eQWri9xsrM
2fD+R9IzkQD4wlcgMGRj+p2GCsRtW+NlE3+uAsEMcb379l76f/jLINEN2c3VWpU3
QV3bFzkdrSpTg7eRGrlkL2RQ7NrxebjswQNwr7jzLJrh5YJFQi2xFT/IGZ3tgfib
NIzWViFyuEABY80/z6yArjo=
-----END PRIVATE KEY-----
)EOF";