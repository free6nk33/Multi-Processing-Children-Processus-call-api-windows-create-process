Le programme créer des processus enfants pour interragir avec l'antivirus de windows (AV) en créant des fichiers temporaires en y écrivant du contenu simulé, puis en supprimant ces fichiers après un délai.Chaque processus enfant réalise cette action plusieurs fois pour à la fin lancer un processus externe (notepad.exe).

Fonctionnalités:

    - Simulation multi-thread process
    - Executation de processus externe
    - Utilisation de l'API Windows


Execution:

    g++ -o antivirus_simulator antivirus_simulator.cpp -lpthread