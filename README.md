# Progetto Progettazione di Sistemi Elettronici - A.A. 2023/2024

## Il progetto
Il progetto consiste nella realizzazione di un sistema di tracciamento della posizione e di un sistema per indicare la posizione di un punto nello spazio.

## Il gruppo
- **Andrea Farinaro**
- **Alan Masutti**
- **Federico Peruzzo**
  
## Il materiale
Il progetto si basa su una scheda progettata e realizzata durante il corso e si basa su un microcontrollore RP2040 della Raspberry Pi.

## Beta Version
E' disponibile questa versione beta del progetto, purtroppo non comprende ancora l'implementazione della bussola perchè putroppo non siamo riusciti a trovare un modo per pulire il segnale. Per il resto il progetto è funzionante.

## Come compilare il progetto
Per compilare il progetto è necessario avere installato VSCode ed il plugin PlatformIO, una volta installato basta aprire la cartella del progetto, attendere che vengano installate le dipendenze e compilare il progetto scegliendo il profilo `env:pico`, mentre il profilo `env:esp01` per caricare il codice sulla scheda ESP01 mediante il convertitore USB-TTL.

Tutti gli altri profili sono stati utilizzati per testare il codice durante lo sviluppo, infatti implementano o testano singole funzionalità del progetto.