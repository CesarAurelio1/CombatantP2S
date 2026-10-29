// Codes infrarouges attendus
#define DEMARRAGE 0x11  // Code pour la demande de démarrage initiale du robot
#define REVEIL 0x22     // Code pour la demande de réveil du robot lors de la détection du bleu

// Demandes qui seront traitées par le programme principal
bool demandeDemarrage = false;
bool demandeReveil = false;

void lireManette(uint32_t message);
void beep(int count);