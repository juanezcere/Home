#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <EEPROM.h>

const char HTTP_HEAD[] PROGMEM = "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'/><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/><meta name='description' content='Zabavy'/><meta name='keywords' content='Zabavy'/><meta name='author' content='Juanez'/><title>Zabavy</title>";
const char HTTP_STYLE[] PROGMEM = "<style>* {font-family: 'Quattrocento Sans', sans-serif; padding: 0px; margin: 0px;} body {text-align: center; background: #E6E6E6;} header {position: fixed; width: 100%; float: left; background: rgb(169, 3, 41); background: -moz-linear-gradient(top, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); background: -webkit-linear-gradient(top, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); background: linear-gradient(to bottom, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#a90329', endColorstr='#6d0019', GradientType=0); box-shadow: 0px 3px 5px; text-align: center; height: 40px; padding-top: 15px; padding-button: 15px;} header p {text-decoration: none; color: #FFFFFF; font-size: 20px; font-weight: bold; border-radius: 3px; -moz-border-radius: 3px; -webkit-border-radius: 3px;} section {padding: 60px;} div {color: #000000; padding: 20px;} button {background: rgb(169, 3, 41); background: -moz-linear-gradient(top, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); background: -webkit-linear-gradient(top, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); background: linear-gradient(to bottom, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#a90329', endColorstr='#6d0019', GradientType=0); box-shadow: 0px 0px 20px rgba(0, 0, 0, 0.5); padding: 10px; border-radius: 20px; -moz-border-radius: 20px; -webkit-border-radius: 20px; margin: 20px; transition: all 0.5s linear; color: #FFFFFF; float: center; width: 100px; height: 60px;} footer {width: 100%; height: 30px; float: left; margin: auto; margin-bottom: 0px; bottom: 0px; left: 0px; background: rgb(169, 3, 41); background: -moz-linear-gradient(top, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); background: -webkit-linear-gradient(top, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); background: linear-gradient(to bottom, rgba(169, 3, 41, 1) 0%, rgba(143, 2, 34, 1) 44%, rgba(109, 0, 25, 1) 100%); filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#a90329', endColorstr='#6d0019', GradientType=0); text-align: center; position: fixed;} footer p {text-decoration: none; color: #FFFFFF; font-size: 15px; border-radius: 3px; -moz-border-radius: 3px; -webkit-border-radius: 3px; padding: 7px;} table {margin: auto; text-align: left;} .l {background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAYAAABXAvmHAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAZjQAAGY0BJ9e2RgAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAALLSURBVGiB7di9i1VHGAbw36yaIhvZbEK+CDHBfEhShSAhRVLZhiQIgUACCfkDUiSQkDRpQjobwT/AQgs7C+1ERVZUxGJFFdUVC1dRxLuFun6Oxe6KXM/svTPn4xb6wBT3PTPP+zxzz7wzZ0KMUZsIIUzia3yDD/E2XsUNXMIMdmFnjPFqdoIYYysNL2AL7iIO0R5gO1Zn5WlJ/Ds4PKTw/nYS60ZmABtwrVD8UpvDt50bwEbcryl+qT3Er4NyhqYWcQhhBU5g3TLdjuIsbuBlvI/PERL9r2BtjPF2krHB2f9Jejan8XFi3LuYWmbsH62/QliB0wkB2/DigPGrsDkx/irG2zaQmv0TGMvg2Zvg+attA7sTiX/I5PkqwXOsbQNnKpKeypn9J7j2VXD1Uv3Hkqt7SCxWn/cqHu2PMT4soJyqiE2EEF6p6lzbANZgFfpxvpDvQiK+tirYhIEPEvGZQr7UuNYMvJUpZBBS49ZUBZswkOJI757Lo5eIrxw6mIlz2FoRnyvku5vgO17VubGz0KhQ/A+EEF7DF/gU440pSmMe0zgUY5wFULDRjOF33NLMsTm33cO/WFm0E2MLRiH8qUNitgFswKiFP9k25pbRnzP7t41fcg2sb0VGOdZnldEQQg8T7enJRxM78Ujx3EABLmEPLjbClllGe8pL3m199zz4viZn9iZWJ9lvCc4f6xjoqgrdt3BpO1/BGTCH1QW8na2B61XiIcYYMVv1bBh0ZeCNEELlp2cI4XV8VErcZRX6Z/F16cff0nejg9HhIo74v4/vz5p8set94JMBv7PxfCceNbo20J+vfv6OF3HP4l0/VuFyTb7YxL1QDiZwMISwA9/hzbqEz9wHTeVxYIS4k2tguhUZ5ZjONXCgFRnlOJBbhcZxDrUqR0NtFpMlN3OfYQajFv9l0dXioomX8B+mcLMj0fM4gk2YXNLyCANG+kd/3EjsAAAAAElFTkSuQmCC') no-repeat left center; background-size: 1em;} .q {float: right; width: 64px; text-align: right;} input {border-radius: 10px; -moz-border-radius: 10px; -webkit-border-radius: 10px; padding: 5px; margin: 20px auto; box-shadow: 0 3px 5px; width: 250px; height: 25px; color: #000000; text-align: center;}</style>";
const char HTTP_SCRIPT[] PROGMEM = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEADEND[] PROGMEM = "</head><body>";
const char HTTP_HEADER[] PROGMEM = "<header><p>Zabavy</p></header>";
const char HTTP_ROOT[] PROGMEM = "<section><div><p>Bienvenido a Zabavy.</br> Seleccione una opción</p></div><br/><form action='/wifi' metod='get'><button>Configurar WiFi</button></form><br/><form action='/info' metod='get'><button>Información</button></form></section>";
const char HTTP_FOOTER[] PROGMEM = "<footer><p>Zabavy INC. Todos los derechos reservados.</p></footer>";
const char HTTP_NONET[] PROGMEM = "<section><div><p>No hay redes WiFi visibles en este momento, recargue esta página para volver a buscar redes WiFi.</p></div></section>";
const char HTTP_UNSAVED[] PROGMEM = "<section><div><p>Las configuraciones del control no pudieron ser guardadas.<br/>Si el problema persiste comuniquese con nosotros.</p></div></section>";
const char HTTP_ITEM[] PROGMEM = "<tr><td><a href='#p' onclick='c(this)'>{v}</a></td><td><span class='q {i}'>{r}%</span></td></tr>";
const char HTTP_FORM[] PROGMEM = "<div><form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='NOMBRE DE RED' required/><br/><input id='p' name='p' length=64 type='password' placeholder='CONTRASEÑA' required/><br/><button type='submit'>Guardar</button></form></div>";
const char HTTP_INFO[] PROGMEM = "<tr><td><p>{v}</p></td><td><p>{r}</p></td></tr>";
const char HTTP_RESET[] PROGMEM = "<section><div><p>Reiniciando control.</p></div></section>";
const char HTTP_NOFOUND[] PROGMEM = "<section><div><p>UPS... Al parecer esta página no se encuentra disponible.</p></div></section>";
const char HTTP_END[] PROGMEM = "</body></html>";

#define APSSID "ZABAVY HOME"
#define LED 16

WiFiClient cliente;
ESP8266WebServer webServer(80);
Ticker ticker;
String str_SSID = "", str_PASS = "";
boolean state = false, configured = false;

void configure(boolean estado) {
  configured = true;
  if (estado) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(str_SSID.c_str(), str_PASS.c_str());
    Serial.println(F("MODO ESTACION"));
    Serial.print(F("CONECTANDO CON \""));
    Serial.print(str_SSID.c_str());
    Serial.println(F("\""));
    Serial.print(F("IP LOCAL: "));
    Serial.println(WiFi.localIP());
  } else {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(APSSID);
    WiFi.softAPConfig(IPAddress(10, 0, 0, 1), IPAddress(10, 0, 0, 1), IPAddress(255, 255, 255, 0));
    Serial.println(F("MODO PUNTO DE ACCESO"));
    Serial.println(F("NOMBRE DE RED: "));
    Serial.println(APSSID);
    Serial.print(F("IP SERVIDOR: "));
    Serial.println(WiFi.softAPIP());
  }
}

boolean readConfig(void) {
  Serial.println(F("BUSCANDO CONFIGURACION"));
  if ((EEPROM.read(0) == 'Z') && (EEPROM.read(1) == 'A') && (EEPROM.read(2) == 'B') && (EEPROM.read(3) == 'A') && (EEPROM.read(4) == 'V') && (EEPROM.read(5) == 'Y')) {
    Serial.println(F("DATOS ENCONTRADOS"));
    str_SSID = "";
    str_PASS = "";
    int i = 6;
    while (EEPROM.read(i) != 0xFF) {
      str_SSID += char(EEPROM.read(i));
      i++;
    }
    i++;
    Serial.print(F("SSID: "));
    Serial.println(str_SSID);
    while (EEPROM.read(i) != 0xFF) {
      str_PASS += char(EEPROM.read(i));
      i++;
    }
    Serial.print(F("PASS: "));
    Serial.println(str_PASS);
    return true;
  } else {
    Serial.println(F("NO SE ENCONTRO NINGUNA CONFIGURACION"));
    return false;
  }
}

int getRSSIasQuality(int RSSI) {
  int quality = 0;
  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

void formatMemory(void) {
  Serial.println(F("FORMATEANDO MEMORIA INTERNA"));
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0x00);
    Serial.print(F("."));
  }
  EEPROM.commit();
  Serial.println();
}

void pageRoot(void) {
  Serial.println(F("CARGANDO PAGINA INICIAL"));
  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEADEND);
  page += FPSTR(HTTP_HEADER);
  page += FPSTR(HTTP_ROOT);
  page += FPSTR(HTTP_FOOTER);
  page += FPSTR(HTTP_END);
  webServer.send(200, "text/html", page);
  Serial.println(F("PAGINA INICIAL ENVIADA"));
}

void pageWifi(void) {
  Serial.println(F("CARGANDO PAGINA DE CONFIGURACION DEL WIFI"));
  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEADEND);
  page += FPSTR(HTTP_HEADER);
  Serial.println(F("BUSCANDO REDES WIFI"));
  int n = WiFi.scanNetworks();
  if (n == 0) {
    page += FPSTR(HTTP_NONET);
  } else {
    int indices[n];
    for (int i = 0; i < n; i++) {
      indices[i] = i;
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }
    String cssid;
    for (int i = 0; i < n; i++) {
      if (indices[i] == -1) continue;
      cssid = WiFi.SSID(indices[i]);
      for (int j = i + 1; j < n; j++) {
        if (cssid == WiFi.SSID(indices[j])) {
          indices[j] = -1;
        }
      }
    }
    page += F("<section><table>");
    Serial.println(F("ORDENANDO REDES WIFI"));
    for (int i = 0; i < n; i++) {
      if (indices[i] == -1) continue;
      int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));
      if (quality > -1) {
        String item = FPSTR(HTTP_ITEM);
        String rssiQ;
        rssiQ += quality;
        item.replace("{v}", WiFi.SSID(indices[i]));
        item.replace("{r}", rssiQ);
        if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE) {
          item.replace("{i}", "l");
        } else {
          item.replace("{i}", "");
        }
        page += item;
      }
    }
    page += F("</table>");
  }
  page += FPSTR(HTTP_FORM);
  page += F("</section>");
  page += FPSTR(HTTP_FOOTER);
  page += FPSTR(HTTP_END);
  webServer.send(200, "text/html", page);
  Serial.println(F("PAGINA DE CONFIGURACION WIFI ENVIADA"));
}

void saveWifi(void) {
  Serial.println(F("CARGANDO PAGINA DE GUARDADO DE WIFI"));
  str_SSID = webServer.arg("s").c_str();
  str_PASS = webServer.arg("p").c_str();
  Serial.println(F("DATOS OBTENIDOS"));
  Serial.print(F("SSID: "));
  Serial.println(str_SSID);
  Serial.print(F("PASS: "));
  Serial.println(str_PASS);
  formatMemory();
  int j = str_SSID.length() + str_PASS.length() + 2;
  char datos[j];
  int i = 0;
  for (int h = 0; h < str_SSID.length(); h++) {
    datos[i] = str_SSID.charAt(h);
    i++;
  }
  datos[i] = 0xFF;
  i++;
  for (int h = 0; h < str_PASS.length(); h++) {
    datos[i] = str_PASS.charAt(h);
    i++;
  }
  datos[i] = 0xFF;
  for (i = 6; i < (j + 6); i++) {
    EEPROM.write(i, datos[i - 6]);
  }
  EEPROM.write(0, 'Z');
  EEPROM.write(1, 'A');
  EEPROM.write(2, 'B');
  EEPROM.write(3, 'A');
  EEPROM.write(4, 'V');
  EEPROM.write(5, 'Y');
  EEPROM.commit();
  Serial.println(F("DATOS GUARDADOS EN MEMORIA INTERNA"));
  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEADEND);
  page += FPSTR(HTTP_HEADER);
  boolean y = false;
  if ((EEPROM.read(0) == 'Z') && (EEPROM.read(1) == 'A') && (EEPROM.read(2) == 'B') && (EEPROM.read(3) == 'A') && (EEPROM.read(4) == 'V') && (EEPROM.read(5) == 'Y')) {
    Serial.println(F("CARGANDO CONFIGURACION"));
    WiFi.begin(str_SSID.c_str(), str_PASS.c_str());
    page += F("<section><div><p>");
    page += WiFi.localIP().toString();
    page += F("</p></div></section>");
    Serial.print(F("DIRECCION IP DEL MODULO: "));
    Serial.println(WiFi.localIP());
    y = true;
  } else {
    Serial.println(F("ERROR GUARDANDO INFORMACION EN LA MEMORIA INTERNA"));
    page += FPSTR(HTTP_UNSAVED);
    state = false;
  }
  page += FPSTR(HTTP_FOOTER);
  page += FPSTR(HTTP_END);
  webServer.send(200, "text/html", page);
  Serial.println(F("PAGINA DE GUARDAR CONFIGURACION ENVIADA"));
  if (y) state = true;
  delay(10000);
  if (state) configured = false;
}

void pageInfo(void) {
  Serial.println(F("CARGANDO PAGINA DE INFORMACION"));
  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEADEND);
  page += FPSTR(HTTP_HEADER);
  page += F("<section><table>");
  String item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("ID DEL CHIP"));
  item.replace("{r}", String(ESP.getChipId()));
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("FLASH DEL CHIP"));
  item.replace("{r}", String(ESP.getFlashChipSize()) + " BYTES");
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("TAMAÑO DE LA MEMORIA FLASH"));
  item.replace("{r}", String(ESP.getFlashChipRealSize()) + " BYTES");
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("VERSIÓN DEL CÓDIGO"));
  item.replace("{r}", F("0.9.A"));
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("NOMBRE DEL ACCESS POINT"));
  item.replace("{r}", APSSID);
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("DIRECCIÓN IP DEL SERVIDOR"));
  item.replace("{r}", WiFi.softAPIP().toString());
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("DIRECCIÓN MAC DEL SERVIDOR"));
  item.replace("{r}", WiFi.softAPmacAddress());
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("ESTADO DE LA CONEXIÓN WIFI"));
  item.replace("{r}", WiFi.status() == WL_CONNECTED ? F("CONECTADO") : F("DESCONECTADO"));
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("NOMBRE DE LA RED"));
  item.replace("{r}", WiFi.status() == WL_CONNECTED ? str_SSID : F("SIN CONEXIÓN"));
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("CONTRASEÑA DE LA RED"));
  item.replace("{r}", WiFi.status() == WL_CONNECTED ? F("*******") : F("SIN CONTRASEÑA"));
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("DIRECCIÓN IP DE LA ESTACIÓN"));
  item.replace("{r}", WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : F("SIN DIRECCIÓN IP"));
  page += item;
  item = FPSTR(HTTP_INFO);
  item.replace("{v}", F("DIRECCIÓN MAC DE LA ESTACIÓN"));
  item.replace("{r}", WiFi.macAddress());
  page += item;
  page += F("</table></section>");
  page += FPSTR(HTTP_FOOTER);
  page += FPSTR(HTTP_END);
  webServer.send(200, "text/html", page);
  Serial.println(F("PAGINA DE INFORMACION ENVIADA"));
}

void pageState(void) {
  Serial.println(F("PONIENDO ESTADO"));
  boolean y = false;
  String z = webServer.arg("z").c_str();
  Serial.println(z);
  if (z == "1") {
    y = true;
  }
  digitalWrite(LED, y ? HIGH : LOW);
  webServer.send(200, "text/html", (digitalRead(LED)) ? "1" : "0");
  Serial.println(F("PAGINA ENVIADA"));
}

void appWifi(void) {
  Serial.println(F("CARGANDO PAGINA DE CONFIGURACION DEL WIFI PARA APP"));
  String page = F("{");
  int redes = 0;
  Serial.println(F("BUSCANDO REDES WIFI"));
  int n = WiFi.scanNetworks();
  if (n == 0) {
    page += F("0}[]");
  } else {
    int indices[n];
    for (int i = 0; i < n; i++) {
      indices[i] = i;
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }
    String cssid;
    for (int i = 0; i < n; i++) {
      if (indices[i] == -1) continue;
      cssid = WiFi.SSID(indices[i]);
      for (int j = i + 1; j < n; j++) {
        if (cssid == WiFi.SSID(indices[j])) {
          indices[j] = -1;
        }
      }
    }
    Serial.println(F("ORDENANDO REDES WIFI"));
    String subPage = "";
    for (int i = 0; i < n; i++) {
      if (indices[i] == -1) continue;
      int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));
      if (quality > -1) {
        redes++;
        subPage += F("[");
        subPage += WiFi.SSID(indices[i]);
        subPage += F("]");
      }
    }
    page += redes;
    page += F("}");
    page += subPage;
    page += F(";");
    webServer.send(200, "text/html", page);
    Serial.println(F("PAGINA DE CONFIGURACION WIFI PARA APP ENVIADA"));
  }
}

void appSave(void) {
  str_SSID = webServer.arg("s").c_str();
  str_PASS = webServer.arg("p").c_str();
  Serial.println(F("DATOS OBTENIDOS"));
  Serial.print(F("SSID: "));
  Serial.println(str_SSID);
  Serial.print(F("PASS: "));
  Serial.println(str_PASS);
  formatMemory();
  int j = str_SSID.length() + str_PASS.length() + 2;
  char datos[j];
  int i = 0;
  for (int h = 0; h < str_SSID.length(); h++) {
    datos[i] = str_SSID.charAt(h);
    i++;
  }
  datos[i] = 0xFF;
  i++;
  for (int h = 0; h < str_PASS.length(); h++) {
    datos[i] = str_PASS.charAt(h);
    i++;
  }
  datos[i] = 0xFF;
  for (i = 6; i < (j + 6); i++) {
    EEPROM.write(i, datos[i - 6]);
  }
  EEPROM.write(0, 'Z');
  EEPROM.write(1, 'A');
  EEPROM.write(2, 'B');
  EEPROM.write(3, 'A');
  EEPROM.write(4, 'V');
  EEPROM.write(5, 'Y');
  EEPROM.commit();
  Serial.println(F("DATOS GUARDADOS EN MEMORIA INTERNA"));
  boolean y = false;
  if ((EEPROM.read(0) == 'Z') && (EEPROM.read(1) == 'A') && (EEPROM.read(2) == 'B') && (EEPROM.read(3) == 'A') && (EEPROM.read(4) == 'V') && (EEPROM.read(5) == 'Y')) {
    Serial.println(F("CARGANDO CONFIGURACION"));
    WiFi.begin(str_SSID.c_str(), str_PASS.c_str());
    Serial.print(F("DIRECCION IP DEL MODULO: "));
    Serial.println(WiFi.localIP());
    y = true;
  } else {
    Serial.println(F("ERROR GUARDANDO INFORMACION EN LA MEMORIA INTERNA"));
    state = false;
  }
  webServer.send(200, "text/html", WiFi.localIP().toString());
  Serial.println(F("PAGINA DE GUARDAR CONFIGURACION ENVIADA"));
  if (y) state = true;
  delay(10000);
  if (state) configured = false;
}

void pageNoFound(void) {
  Serial.println(F("PAGINA NO ENCONTRADA"));
  String page = FPSTR(HTTP_HEAD);
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  page += FPSTR(HTTP_HEADEND);
  page += FPSTR(HTTP_HEADER);
  page += FPSTR(HTTP_NOFOUND);
  page += FPSTR(HTTP_FOOTER);
  page += FPSTR(HTTP_END);
  webServer.send(404, "text/html", page);
  Serial.println(F("PAGINA NO ENCONTRADA ENVIADA"));
}

void tick(void) {
  if (state) {
    if (!configured) {
      configure(true);
    }
  } else {
    if (!configured) {
      configure(false);
    }
  }
}

void setup(void) {
  EEPROM.begin(512);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(100);
  Serial.println();
  state = readConfig();
  Serial.println(F("CONFIGURANDO SERVIDOR"));
  webServer.on("/", pageRoot);
  webServer.on("/wifi", pageWifi);
  webServer.on("/wifisave", saveWifi);
  webServer.on("/info", pageInfo);
  webServer.on("/cambio", pageState);
  webServer.on("/wifis", appWifi);
  webServer.on("/save", appSave);
  webServer.onNotFound(pageNoFound);
  webServer.begin();
  Serial.println(F("CONFIGURACION TERMINADA"));
  ticker.attach(1, tick);
}

void loop(void) {
  webServer.handleClient();
  if (Serial.available() > 0) {
    char c = 0x00;
    while (Serial.available() > 0) {
      Serial.read();
    }
    Serial.flush();
    formatMemory();
  }
}
