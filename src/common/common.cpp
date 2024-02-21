#include "common.h"

String splitString(String str, char sep, int index) {
  /* str a' la variabile di tipo String che contiene il valore da splittare
     sep a' ia variabile di tipo char che contiene il separatore (bisoga usare l'apostrofo: splitString(xx, 'xxx', yy)
     index a' la variabile di tipo int che contiene il campo splittato: str = "11111:22222:33333" se index= 0;
                                                                       la funzione restituira': "11111"
  */
  int found = 0;
  int strIdx[] = { 0, -1 };
  int maxIdx = str.length() - 1;

  for (int i = 0; i <= maxIdx && found <= index; i++) {
    if (str.charAt(i) == sep || i == maxIdx) {
      found++;
      strIdx[0] = strIdx[1] + 1;
      strIdx[1] = (i == maxIdx) ? i + 1 : i;
    }
  }
  return found > index ? str.substring(strIdx[0], strIdx[1]) : "";
}

// String popolareDatiDaMandare(){
//   String d;
//   WayPoint_t dati;
//   dati.timeInfo_1 = getRMCData()->timeInfo;
//   dati.latitude_1 = getGGAData()->latitude;
//   strcpy(getGGAData()->longitude,dati.longitude_1);
//   dati.fix_1 = getGGAData()->fix;
  
//   return d;
// }
 