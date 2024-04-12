# Upute za instalaciju

### Windows

1. Preuzeti PortAudio s adrese: https://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz
2. U korijenskom direktoriju Qt projekta

    ```bash
    diplomski-rad/Software/audio-visualizer    
    ```
    
    kreirati mapu npr. naziva `libs` (ne `lib` jer iz nekog razloga stvara problem s obzirom da PortAudio biblioteka također sadrži mapu istog naziva) te smjestiti preuzetu mapu PortAudio biblioteke
    
3. Preuzeti MSYS2 s adrese: https://www.msys2.org/

    Riječ je o Linux-like bash shellu za Windows, potrebno za konfiguraciju biblioteke
    
4. Otvoriti MSYS2 MINGW64 te napraviti sljedeće:

    * Instalirati make:
    
        ```bash
        pacman -S make
        ```
    
    * Podesiti varijablu okoline za MINGW64 kompajler (potrebno zbog gcc-a i g++-a):
    
        Primjer:
        
        ```bash
        export PATH=${PATH}:/C/Dev-Cpp/MinGW64/bin
        ```
        
        Ako nemate Dev Cpp, onda se MINGW64 može pronaći na: https://winlibs.com/ 
        
        Trenutna stabilna verzija: https://github.com/brechtsanders/winlibs_mingw/releases/download/13.2.0posix-17.0.6-11.0.1-ucrt-r5/winlibs-i686-posix-dwarf-gcc-13.2.0-llvm-17.0.6-mingw-w64ucrt-11.0.1-r5.zip
    
    * Smjestiti se u direktorij gdje se nalazi PortAudio:
    
        ```bash
        diplomski-rad/Software/audio-visualizer/libs/portaudio    
        ```
        
    * Sada se mogu izvršiti naredbe, redom:
    
        ```bash
        ./configure
        
        make
        
        make install
        ```
        
        ⚠️VAŽNO: make može potrajati dosta dugo, treba pričekati dokle god ne završi do kraja!

5. Nakon instalacije, ažurirati putanje u .pro datoteci, sekcija `---PORTAUDIO---`, po potrebi
<br/><br/>
👉🏽 VAŽNO: Ako se pokreće .exe (debug ili release) izvan Qt Creator okruženja, javit će se greška da nije pronađen libportaudio-2.dll. Datoteka se nalazi u libs/portaudio/lib/.libs i treba ju smjestiti zajedno s .exe datotekom

### Linux
