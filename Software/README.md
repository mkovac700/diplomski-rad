# Upute za instalaciju i korištenje

https://www.portaudio.com/docs/v19-doxydocs/tutorial_start.html

## Windows

### Instalacija

1. Preuzeti PortAudio s adrese: https://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz
2. U korijenskom direktoriju Qt projekta

    ```bash
    diplomski-rad/Software/audio-visualizer    
    ```
    
    kreirati mapu npr. naziva `libs` (ne `lib` jer iz nekog razloga stvara problem s obzirom da PortAudio biblioteka također sadrži mapu istog naziva) te smjestiti preuzetu (i izdvojenu) mapu PortAudio biblioteke
    
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

### Korištenje

👉🏽 Mikrofon bi trebao biti automatski prepoznat. Za monitoring output uređaja, potrebno je omogućiti Stereo Mix. 

## Linux

### Instalacija

1. Preuzeti PortAudio s adrese: https://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz
2. U korijenskom direktoriju Qt projekta

    ```bash
    diplomski-rad/Software/audio-visualizer    
    ```
    
    kreirati mapu npr. naziva `libs` (ne `lib` jer iz nekog razloga stvara problem s obzirom da PortAudio biblioteka također sadrži mapu istog naziva) te smjestiti preuzetu (i izdvojenu) mapu PortAudio biblioteke

3.  Smjestiti se u direktorij gdje se nalazi PortAudio:

    ```bash
    diplomski-rad/Software/audio-visualizer/libs/portaudio    
    ```
    
4. Instalirati audio drivere:

    * ALSA:

        Riječ je o driveru za Linux audio uređaje. Rješava problem s nemogućnosti pronalaska audio uređaja. Više o tome: https://www.portaudio.com/docs/v19-doxydocs/compile_linux.html#:~:text=Installing%20ALSA%20Development%20Kit
        
        Dodatne informacije o ALSA-i: https://www.alsa-project.org/wiki/Main_Page
        
        Za instalaciju izvršiti naredbu:
        
        ```bash
        sudo apt-get install libasound-dev
        ```
    
    * JACK:
    
        Više informacija: https://jackaudio.org/
    
        ```bash
        sudo apt-get install libjack-dev
        ```
        
    * PULSE AUDIO:
    
        Omogućava simultano korištenje audio uređaja. Rješava problem s nemogućnosti pronalaska mikrofona. Također omogućava monitoring output uređaja.
        
        ```bash
        sudo apt-get install pulseaudio pulseaudio-utils
        
        sudo apt-get install pavumeter pavucontrol paprefs ubuntu-sounds
        ```
        
        ⚠️VAŽNO: Nakon instalacije napraviti restart!
        
5. Izvršiti sljedeće naredbe:

    ⚠️VAŽNO: Prije izvšavanja mora biti instalirana ALSA i JACK, kako bi ih skripta configure mogla pronaći

    ```bash
    ./configure && make   
    
    sudo make install
    ```

6. Pokušati izvršiti build.

    * Ako se javi greška `cannot find -lGL` znači da nije instaliran OpenGL, pa je potrebno izvršiti:
    
        ```bash
        sudo apt-get install libgl-dev libglfw3-dev
        ```

### Korištenje

Za korištenje mikrofona koristiti sljedeće postavke:    

* U Sound Preferences:

    * `Input > Built-in Audio Analog Stereo > Connector: Microphone / Microphone 1`
    * `Output > bilo koja opcija`
    
* U aplikaciji:

    * `Intel 82801AA-ICH` (trebao bi biti već zadan)
    * Sample rate: 48000
        
Za korištenje monitoringa output uređaja:

* U Sound Preferences:

    * `Input > Monitor Source of Simultaneous output to Built-in Audio Analog Stereo`
    * `Output > Simultaneous output to Built-in Audio Analog Stereo`
    
* U aplikaciji:

    * Za ulaz i izlaz odabrati uređaj `pulse`
    * Sample rate: 44100
