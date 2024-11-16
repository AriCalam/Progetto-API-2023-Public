//Progetto di API A.A.2022-2023
//Pianificazione percorso tra stazioni di un'autostrada con macchine elettriche

//LIBRERIE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//DEFINIZIONE DELLA LISTA PER LE AUTOMOBILI DI OGNI STAZIONE
typedef struct elemento_lista *lista;

struct elemento_lista {
    int autonomia;
    lista succ;
}; //Struttura di un elemento della lista

//DEFINIZIONE DEL BST PER LE STAZIONI
typedef struct bst* puntatore_albero; //Definisco i puntatori ai nodi dell'albero

struct bst {
    int numero_stazione;
    puntatore_albero dx;
    puntatore_albero sx;
    puntatore_albero padre;
    struct elemento_lista *automobili;
    struct elemento_lista *max_autonomia;
}; //Struttura di un nodo dell'albero

//FUNZIONI PER LA GESTIONE DEL BST

/**
 * Crea un nodo da inserire in un BST.
 * @param stazione il numero della stazione.
 * @param automobili il puntatore alla lista di automobili della stazione.
 * @param max il puntatore all'automobile con l'autonomia massima nella stazione.
 * @param padre il puntatore al nodo padre.
 * @return il nodo creato.
 */
puntatore_albero creaNodo(int stazione, lista automobili, lista max, puntatore_albero padre) {
    puntatore_albero nodo = malloc(sizeof(struct bst));
    nodo->dx = NULL;
    nodo->sx = NULL;
    nodo->padre = padre;
    nodo->numero_stazione = stazione;
    nodo->automobili = automobili;
    nodo->max_autonomia = max;
    return nodo;
}

/**
 * Aggiunge un nodo/stazione ad un BST.
 * Stampa sul file di output 'aggiunta' oppure 'non-aggiunta'.
 * @param radice il puntatore alla radice dell'albero in cui inserire il nodo.
 * @param stazione il numero della stazione da inserire.
 * @param automobili il puntatore alla lista di automobili della stazione.
 * @param max il puntatore all'automobile con l'autonomia massima nella stazione.
 * @param out il file su cui scrivere il risultato
 */
puntatore_albero inserimentoBST(puntatore_albero radice, int stazione, struct elemento_lista *automobili, struct elemento_lista *max) {
    if(radice == NULL) { //Se l'albero è vuoto
        //printf("Sono in albero vuoto\n");
        puntatore_albero nodo = NULL;
        nodo = creaNodo(stazione, automobili, max, NULL);
        //printf("Albero era vuoto: stazione %d aggiunta\n", stazione);
        printf("aggiunta\n");
        return nodo;
    }
    else {
        puntatore_albero prev = NULL;
        puntatore_albero curr = radice;

        while(curr != NULL) {
            prev = curr;
            if(stazione == curr->numero_stazione) {
                //printf("Stazione %d già esistente, non aggiunta\n", stazione);
                printf("non aggiunta\n");
                return radice;
            }
            else if(stazione < curr->numero_stazione)
                curr = curr->sx;
            else
                curr = curr->dx;
        }

        if(stazione < prev->numero_stazione) {
            prev->sx = creaNodo(stazione, automobili, max, prev);
        }
        else {
            prev->dx = creaNodo(stazione, automobili, max, prev);
        }
    }

    //printf("Stazione %d aggiunta\n", stazione);
    printf("aggiunta\n");
    return radice;
}

/**
 * Stampa le autonomie di tutte le auto contenute in un nodo.
 * @param stazione la stazione di cui stampare le autonomie.
 */
int stampaAutoInStazione(puntatore_albero stazione) {
    struct elemento_lista *corrente = stazione->automobili;
    int contaAuto = 0;

    printf("Autonomie:");
    while(corrente != NULL) {
        //printf("\n\t\t");
        printf(" %d", corrente->autonomia);
        corrente = corrente->succ;
        contaAuto++;
    }
    //printf("\n");
    printf(", ");

    free(corrente);
    return contaAuto;
}

/**
 * Stampa tutte le informazioni di una stazione (numero, massima autonomia. auto), contando anche quante auto sono
 *  salvate in essa.
 * @param stazione la stazione di cui stampare le informazioni.
 */
void stampaStazione(puntatore_albero stazione) {
    int contaAuto;

    printf("\t");
    printf("Stazione %d, ", stazione->numero_stazione);
    if(stazione->max_autonomia != NULL)
        printf("Max-autonomia %d, ", stazione->max_autonomia->autonomia);
    contaAuto = stampaAutoInStazione(stazione);
    //printf("\t");
    printf("Tot auto %d, ", contaAuto);
    printf("\n");
}

/**
 * Visita dell'albero "inorder-tree-walk" dall'elemento minore all'elemento maggiore.
 * @param radice la radice dell'albero da visitare.
 */
void visitaBST(puntatore_albero radice) {
    if(radice != NULL) {
        visitaBST(radice->sx);
        //printf(" %d", radice->numero_stazione);
        stampaStazione(radice);
        //printf("\tS=%d, UAA=%d, MAX=%d\n", radice->numero_stazione, radice->automobili->autonomia, radice->max_autonomia->autonomia);
        visitaBST(radice->dx);
    }
}

/**
 * Cerca il minimo in un BST.
 * @param nodo radice dell'albero in cui cercare il minimo.
 * @return il minimo dell'albero la cui radice è stata passata per parametro.
 */
puntatore_albero minimoBST(puntatore_albero nodo) {
    puntatore_albero curr = nodo;
    while(curr->sx != NULL)
        curr = curr->sx;
    return curr;
}

/**
 * Cerca il successore in un BST.
 * Il successore di un nodo x è il nodo y tale che y->key èla più piccola tra le chiavi che sono
 *  più grandi di x->key.
 * @param nodo il nodo di cui cercare il successore.
 * @return il successore del nodo passato per parametro.
 */
puntatore_albero successoreBST(puntatore_albero nodo) {
    if(nodo->dx != NULL)
        return minimoBST(nodo->dx);
    puntatore_albero succ = nodo->padre;
    while(succ != NULL && nodo == succ->dx) {
        nodo = succ;
        succ = succ->padre;
    }
    return succ;
}

/**
 * Cerca un nodo in un BST.
 * @param radice la radice del BST in cui cercare.
 * @param stazione il numero della stazione da cercare nel BST.
 * @return il nodo trovato o la radice dell'albero.
 */
puntatore_albero ricercaNodo(puntatore_albero radice, int stazione) {
    if(radice == NULL || stazione == radice->numero_stazione)
        return radice;
    if(stazione < radice->numero_stazione)
        return ricercaNodo(radice->sx, stazione);
    else
        return ricercaNodo(radice->dx, stazione);
}

/**
 * Elimina un'auto da una stazione, senza stampare avvisi (se non per debug).
 * @param nodo la stazione da cui eliminare l'auto.
 * @param autonomia l'autonomia dell'auto da eliminare.
 */
void eliminaAutoDaNodo(puntatore_albero nodo, int autonomia) {
    //stampaStazione(nodo);
    lista tmp = nodo->automobili;
    lista prev = nodo->automobili;

    if(tmp != NULL && tmp->autonomia == autonomia) { //Se l'autonomia cercata è quella in testa alla lista
        nodo->automobili = nodo->automobili->succ; //Cambio la testa
        free(tmp);
        //printf("Auto %d trovata in testa nella stazione %d: rottamata\n", autonomia, nodo->numero_stazione);
        //printf("rottamata\n");
    }
    else {
        while(tmp != NULL && tmp->autonomia != autonomia) {
            prev = tmp;
            tmp = tmp->succ;
        }

        if(tmp == NULL) { //Se l'auto non è stata trovata nella lista
            //printf("Auto %d non trovata nella stazione %d: non rottamata\n", autonomia, nodo->numero_stazione);
            //printf("non rottamata\n");
            free(tmp);
        }
        else {
            prev->succ = tmp->succ;
            free(tmp);
            //printf("Auto %d trovata nella stazione %d: rottamata\n", autonomia, nodo->numero_stazione);
            //printf("rottamata\n");
        }
    }
}

/**
 * Elimina un nodo da un BST.
 * @param radice la radice dell'albero da cui rimuovere il nodo.
 * @param stazione la stazione da rimuovere dall'albero.
 * @return la radice dell'albero a cui è stato rimosso il nodo.
 */
puntatore_albero eliminaNodo(puntatore_albero radice, int stazione) {
    //da_canc è il nodo effettivamente da eliminare, sottoalb è il nodo con cui lo sostituiamo
    puntatore_albero sottoalb = NULL;
    puntatore_albero da_canc = NULL;
    //Ricerca della stazione nell'albero
    puntatore_albero nodo = ricercaNodo(radice, stazione);
    if(nodo == NULL) { //Se la stazione non viene trovata allora ciao
        //printf("Stazione %d non trovata nell'albero: non demolita\n", stazione);
        printf("non demolita\n");
        return radice;
    }

    //Individuazione del nodo da cancellare, sapendo che da_canc non può avere più di un sottoalbero
    if(nodo->sx == NULL || nodo->dx == NULL) { //Se nodo ha max un sottoalbero, allora il nodo da cancellare è nodo stesso
        //printf("Sono qui 1\n");
        da_canc = nodo;

        lista curr = da_canc->automobili;
        while(curr != NULL) {
            eliminaAutoDaNodo(da_canc, curr->autonomia);
            curr = da_canc->automobili;
        }
        free(curr);

        //printf("Stazione %d demolita\n", da_canc->numero_stazione);
    }
    else {//Altrimenti è il suo successore
        //printf("Sono qui 2\n");
        da_canc = successoreBST(nodo);
    }

    //Individuazione del sottoalbero da spostare
    if(da_canc->sx != NULL) { //Se da_canc ha un sottoalbero, a sottoalb viene assegnata la radice di da_canc
        //printf("Sono qui 3\n");
        sottoalb = da_canc->sx;
    }
    else {//Altrimenti, se da_canc non ha sottoalberi, a sottoalb viene assegnato NULL
        //printf("Sono qui 4\n");
        sottoalb = da_canc->dx;
    }

    //Correzione del riferimento al padre
    if(sottoalb != NULL) {
        //printf("Sono qui 5\n");
        sottoalb->padre = da_canc->padre;
    }

    //Correzione del riferimento del padre
    if(da_canc->padre == NULL) {
        //printf("Sono qui 6\n");
        radice = sottoalb;
    }
    else if(da_canc == da_canc->padre->sx) {
        //printf("Sono qui 7\n");
        da_canc->padre->sx = sottoalb;
    }
    else {
        //printf("Sono qui 8\n");
        da_canc->padre->dx = sottoalb;
    }

    //Copia della chiave
    if(da_canc != nodo) { //Se da_canc è il successore di nodo, la chiave di nodo viene sostituita con quella di da_canc
        //printf("Stazione %d demolita\n", nodo->numero_stazione);
        nodo->numero_stazione = da_canc->numero_stazione;

        lista curr = nodo->automobili;
        while(curr != NULL) {
            eliminaAutoDaNodo(nodo, curr->autonomia);
            curr = nodo->automobili;
        }
        free(curr);

        nodo->automobili = da_canc->automobili;
        nodo->max_autonomia = da_canc->max_autonomia;
    }

    free(da_canc);
    printf("demolita\n");
    return radice;
}

//FUNZIONI DEL PROGETTO

/**
 * Inserisce in testa a una lista l'auto di autonomia passata come parametro.
 * @param testa il puntatore alla testa della lista.
 * @param autonomia l'autonomia da inserire.
 */
void inserisciElementoLista(lista *testa, int autonomia) {
    lista el = malloc(sizeof(struct elemento_lista));
    el->autonomia = autonomia;
    el->succ = (*testa);
    (*testa) = el;
}

/**
 * Quando viene letto il comando 'aggiungi-stazione' viene chiamata questa funzione,
 *  che legge il resto del comando dal file e chiama le funzioni necessarie a salvare i dati.
 * @param in il file di input da cui leggere i dati.
 * @param radice la radice dell'albero a cui aggiungere la stazione.
 * @return la radice dell'albero.
 */
puntatore_albero aggiungiStazione(puntatore_albero radice) {
    int stazione, numero_auto, autonomia;
    lista testa = NULL;
    lista max = NULL;

    if(scanf("%d %d", &stazione, &numero_auto) != EOF) {
        //printf("Stazione letta: %d, numero auto letto: %d", stazione, numero_auto);
        //printf("\n");

        if(numero_auto > 0) {
            //Lettura delle autonomie delle auto
            for(int i=0; i<numero_auto; i++) {
                if(scanf("%d", &autonomia) != EOF) {
                    //printf("\t");
                    //printf("Autonomia letta: %d, ", autonomia);
                    //printf("\n");

                    //Aggiunta delle auto in una lista, con inserimento in testa
                    inserisciElementoLista(&testa, autonomia);

                    //Ricerca del massimo
                    if(max == NULL || autonomia > max->autonomia)
                        max = testa;

                    //printf("Auto aggiunta alla lista: %d ", testa->autonomia);
                }
                else
                    printf("Errore 2");
            }

            //Creazione del nodo con la lista ottenuta e il max trovato
            puntatore_albero nodo = NULL;
            nodo = inserimentoBST(radice, stazione, testa, max);
            return nodo;
        }
        else {
            //printf("Radice %p, stazione %d, prec %p, max %p\n", radice, stazione, testa, max);
            puntatore_albero nodo = NULL;
            nodo = inserimentoBST(radice, stazione, testa, max);
            //printf("Radice %p, stazione %d, prec %p, max %p\n", radice, stazione, testa, max);
            return nodo;
        }
    }
    else {
        printf("Errore 1");
        return radice;
    }
}

/**
 * Quando viene letto il comando 'aggiungi-auto' viene chiamata questa funzione,
 *  che legge il resto del comando dal file e chiama le funzioni necessarie a salvare i dati.
 * @param in il file di input da cui leggere i dati.
 * @param radice la radice dell'albero in cui cercare la stazione.
 * @return la radice dell'albero.
 */
void aggiungiAuto(puntatore_albero radice) {
    int stazione, autonomia;

    if(scanf("%d %d", &stazione, &autonomia) != EOF) {
        puntatore_albero nodo = ricercaNodo(radice, stazione);
        if(nodo == NULL) {
            //printf("Stazione %d non trovata, auto non aggiunta\n", stazione);
            printf("non aggiunta\n");
        }
        else {
            inserisciElementoLista(&nodo->automobili, autonomia);

            //Ricerca del massimo
            if (nodo->max_autonomia == NULL || autonomia > nodo->max_autonomia->autonomia)
                nodo->max_autonomia = nodo->automobili;

            //printf("Auto %d aggiunta alla stazione %d\n", nodo->automobili->autonomia, nodo->numero_stazione);
            printf("aggiunta\n");
            //printf("Nuovo max = %d\n", nodo->max_autonomia->autonomia);
        }
    }
}

/**
 * Quando viene letto il comando 'demolisci-stazione' viene chiamata questa funzione,
 *  che legge il resto del comando dal file e chiama le funzioni necessarie a salvare i dati.
 * @param in il file di input da cui leggere i dati.
 * @param radice la radice dell'albero in cui demolire la stazione.
 * @return la radice dell'albero.
 */
puntatore_albero demolisciStazione(puntatore_albero radice) {
    int stazione;

    if(scanf("%d", &stazione) != EOF) {
        //printf("Devo demolire la stazione %d\n", stazione);
        radice = eliminaNodo(radice, stazione);
    }

    return radice;
}

/**
 * Stampa l'albero, scrivendo il numero della stazione e le autonomie delle auto contenute in essa.
 * @param radice la radice dell'albero da stampare.
 */
void stampaAlbero(puntatore_albero radice) {
    printf("\n");
    printf("Albero:");
    printf("\n");
    visitaBST(radice);
    if(radice != NULL)
        printf("Radice = %d", radice->numero_stazione);
    else
        printf("Albero vuoto\n");
    printf("\n");
}

/**
 * Quando viene letto il comando 'rottama-auto' viene chiamata questa funzione...
 * @param in il file di input da cui leggere i dati.
 * @param radice la radice dell'albero in cui demolire la stazione.
 * @return la radice dell'albero.
 */
void rottamaAuto(puntatore_albero radice) {
    int stazione, autonomia;

    if(scanf("%d %d", &stazione, &autonomia) != EOF) {
        puntatore_albero nodo = ricercaNodo(radice, stazione);
        //printf("Devo rottamare l'auto %d nella stazione %d\n", autonomia, stazione);

        if (nodo == NULL) {
            //printf("Stazione %d non trovata, auto non rottamata\n", stazione);
            printf("non rottamata\n");
        } else {
            //stampaStazione(nodo);
            bool max_eliminato = false;
            lista tmp = nodo->automobili;
            lista prev = nodo->automobili;

            if(tmp != NULL && tmp->autonomia == autonomia) { //Se l'autonomia cercata è quella in testa alla lista
                if(tmp->autonomia == nodo->max_autonomia->autonomia)
                    max_eliminato = true;
                nodo->automobili = nodo->automobili->succ; //Cambio la testa
                free(tmp);
                //printf("Auto %d trovata in testa nella stazione %d: rottamata\n", autonomia, nodo->numero_stazione);
                printf("rottamata\n");
            }
            else {
                while(tmp != NULL && tmp->autonomia != autonomia) {
                    prev = tmp;
                    tmp = tmp->succ;
                }

                if(tmp == NULL) { //Se l'auto non è stata trovata nella lista
                    //printf("Auto %d non trovata nella stazione %d: non rottamata\n", autonomia, nodo->numero_stazione);
                    printf("non rottamata\n");
                    free(tmp);
                }
                else {
                    if(tmp->autonomia == nodo->max_autonomia->autonomia)
                        max_eliminato = true;
                    prev->succ = tmp->succ;
                    free(tmp);
                    //printf("Auto %d trovata nella stazione %d: rottamata\n", autonomia, nodo->numero_stazione);
                    printf("rottamata\n");
                }
            }

            if(max_eliminato) {
                //Cerco il nuovo massimo
                lista curr = nodo->automobili;
                struct elemento_lista *new_max = malloc(sizeof(struct elemento_lista));
                new_max->autonomia = 0;
                new_max->succ = NULL;

                while(curr != NULL) {
                    if(curr->autonomia > new_max->autonomia) {
                        new_max->autonomia = curr->autonomia;
                    }
                    curr = curr->succ;
                }

                nodo->max_autonomia = new_max;
            }
        }
    }
}

/**
 * Legge il resto del comando (memorizzando stazione di partenza e di arrivo) e pianifica il percorso tra le due stazioni.
 * @param in il file da cui leggere i dati.
 * @param radice la radice dell'albero in cui pianificare il percorso.
 */
void pianificaPercorso(puntatore_albero radice) {
    int partenza, arrivo;

    if(scanf("%d %d", &partenza, &arrivo) != EOF) {
        if(partenza == arrivo) {
            //printf("Partenza uguale ad arrivo, percorso trovato: %d\n", partenza);
            printf("%d\n", partenza);
            return;
        }
        else {
            //Inizializzazioni utili
            puntatore_albero nodoPartenza = ricercaNodo(radice, partenza);
            puntatore_albero nodoArrivo = ricercaNodo(radice, arrivo);

            //Controllo che le stazioni siano nell'albero
            /*
            if (nodoPartenza == NULL || nodoArrivo == NULL) {
                if (nodoPartenza == NULL)
                    printf("La stazione %d di partenza non è stata trovata nell'albero\n", partenza);
                else
                    printf("La stazione %d di arrivo non è stata trovata nell'albero\n", arrivo);
                printf("nessun percorso\n");
                return;
            } */

            if (partenza < arrivo) {
                //Visito l'albero dal nodo di partenza a quello di arrivo e conto i nodi
                puntatore_albero curr = nodoPartenza;
                int numNodi = 0;
                while (curr != successoreBST(nodoArrivo)) {
                    numNodi++;
                    curr = successoreBST(curr);
                }

                //printf("Numero nodi = %d\n", numNodi);

                //Inizializzazione elementi utili
                int righe = 2;
                int matrice[righe][numNodi];
                curr = nodoPartenza;

                //Visito di nuovo l'albero da partenza ad arrivo e metto il numero di stazione nella prima riga di una
                // matrice, mentre la max_autonomia nella seconda riga
                for (int i = 0; i < numNodi; i++) {
                    matrice[0][i] = curr->numero_stazione;
                    if (curr->max_autonomia != NULL)
                        matrice[1][i] = curr->max_autonomia->autonomia;
                    else
                        matrice[1][i] = 0;
                    curr = successoreBST(curr);
                }

                //Stampa matrice (per debug)
                /*
                printf("Matrice:\n");
                for(int i=0; i<numNodi; i++) {
                    printf("\tS=%d, MA=%d\n", matrice[0][i], matrice[1][i]);
                }*/

                //Pianifico il percorso: scorro la matrice al contrario e guardo le autonomie
                int ss = matrice[0][numNodi-1]; //SS = Stazione Scelta
                int sdr = matrice[0][numNodi-1]; //SDR = Stazione Da Raggiungere
                //All'inizio la stazione che voglio raggiungere è l'arrivo

                //printf("ss=%d, sdr=%d\n", ss, sdr);

                //Array di stazioni del percorso inizializzato a tutti 0
                int percorso[numNodi];
                for (int i = 0; i < numNodi; i++) {
                    percorso[i] = 0;
                }
                percorso[numNodi-1] = sdr;
                int primoZero = numNodi-2;

                //Calcolo del percorso
                for (int i = primoZero; i >= 0; i--) {
                    int stazioneAttuale = matrice[0][i];
                    int autoAttuale = matrice[1][i];
                    int somma = stazioneAttuale + autoAttuale;
                    //printf("att=%d, matt=%d\n", stazioneAttuale, autoAttuale);

                    //Se la attuale arriva a SS
                    if (somma >= ss) {
                        //printf("Sono qui 2\n");
                        //Controllo se la stazione attuale può raggiungere delle altre stazioni dopo SS
                        for (int j=primoZero; j<numNodi-1; j++) {
                            if (percorso[j] != arrivo) {
                                //printf("j=%d, percorso[j]=%d\n", j, percorso[j]);
                                //printf("sdr=%d, ss=%d\n", sdr, ss);
                                if (percorso[j] == 0) { //Se la cella in cui sono è 0 (e quindi anche le precedenti sono 0)
                                    if (j+1 == numNodi-1) { //La cella successiva è l'arrivo
                                        percorso[j] = stazioneAttuale;
                                        ss = stazioneAttuale;
                                        primoZero = j-1;
                                        break;
                                    }
                                    //Se non è l'arrivo non faccio nulla, lascio 0 nella cella in cui sono
                                } else { //La cella è diversa da 0
                                    if (j + 1 == numNodi - 1) { //La cella successiva è l'arrivo
                                        if (percorso[j - 1] == 0) { //La cella precedente (e quindi tutte le prec) sono 0
                                            if (somma >= percorso[j + 1]) {
                                                percorso[j] = stazioneAttuale;
                                                ss = stazioneAttuale;
                                                primoZero = j-1;
                                                //printf("break 2\n");
                                                break;
                                            } else {
                                                percorso[j-1] = stazioneAttuale;
                                                ss = stazioneAttuale;
                                                primoZero = j-1;
                                                //printf("break 3\n");
                                                break;
                                            }
                                        }
                                    } else { //La cella successiva non è l'arrivo
                                        if (percorso[j] == ss) { //E la cella attuale è la ss
                                            if (somma >= percorso[j + 1]) { //E posso arrivare alla stazione successiva
                                                percorso[j] = 0;
                                                ss = percorso[j + 1];
                                            } else { //Non posso arrivare alla stazione successiva
                                                percorso[j - 1] = stazioneAttuale;
                                                sdr = percorso[j];
                                                ss = stazioneAttuale;
                                                primoZero = j - 2;
                                                //printf("break 4\n");
                                                break;
                                            }
                                        }
                                    }
                                }

                                /*printf("Percorso attuale:");
                                for(int k=0; k < numNodi; k++)
                                    printf(" %d", percorso[k]);
                                printf("\n");
                                printf("+++++++++ sdr=%d, ss=%d\n", sdr, ss);*/
                            }
                            //printf("j:%d ---- sdr=%d, ss=%d\n", j, sdr, ss);
                        }

                        /*printf("Percorso attuale:");
                        for(int k=0; k < numNodi; k++)
                            printf(" %d", percorso[k]);
                        printf("\n");
                        printf("sdr=%d, ss=%d\n", sdr, ss);*/
                    }
                    //printf("-------\n");

                    //i = primoZero;
                }

                //Controllo se ho trovato il percorso
                bool partenzaTrovata = false;
                bool arrivoTrovato = false;
                for (int i = 0; i < numNodi; i++) {
                    if (percorso[i] == partenza)
                        partenzaTrovata = true;
                    if (percorso[i] == arrivo)
                        arrivoTrovato = true;
                }

                //Stampo il percorso
                if (partenzaTrovata && arrivoTrovato) {
                    //printf("Percorso trovato: ");
                    if(partenza == 0) {
                        printf("0 ");
                        for (int i = 1; i < numNodi; i++) {
                            if (percorso[i] != 0 && i != numNodi - 1)
                                printf("%d ", percorso[i]);
                            else if (percorso[i] != 0)
                                printf("%d\n", percorso[i]);
                        }
                    }
                    else {
                        for (int i = 0; i < numNodi; i++) {
                            if (percorso[i] != 0 && i != numNodi - 1)
                                printf("%d ", percorso[i]);
                            else if (percorso[i] != 0)
                                printf("%d\n", percorso[i]);
                        }
                    }

                } else {
                    printf("nessun percorso\n");
                }
            }
            else { //Se partenza > arrivo
                //Visito l'albero dal nodo di arrivo a quello di partenza e conto i nodi
                puntatore_albero curr = nodoArrivo;
                int numNodi = 0;
                while (curr != successoreBST(nodoPartenza)) {
                    numNodi++;
                    curr = successoreBST(curr);
                }

                //Inizializzazione elementi utili
                int righe = 3;
                int matrice[righe][numNodi];
                curr = nodoArrivo;

                //Visito di nuovo l'albero da arrivo a partenza e metto il numero di stazione nella prima riga di una
                // matrice, la max_autonomia nella seconda riga, e i km all'arrivo nella terza riga
                for (int i = numNodi - 1; i >= 0; i--) {
                    matrice[0][i] = curr->numero_stazione; //Stazione
                    //Autonomia
                    if (curr->max_autonomia != NULL)
                        matrice[1][i] = curr->max_autonomia->autonomia;
                    else
                        matrice[1][i] = 0;
                    //
                    matrice[2][i] = matrice[0][i] - arrivo; //Km mancanti all'arrivo
                    curr = successoreBST(curr); //Aggiorno curr
                }

                //Stampa matrice (per debug)
                /*printf("\n");
                printf("Matrice:\n");
                for (int i = 0; i < numNodi; i++) {
                    printf("\tS = %d, MA = %d, Km all'arrivo = %d\n", matrice[0][i], matrice[1][i], matrice[2][i]);
                }
                printf("\n");*/

                //Matrice di stazioni del percorso
                //La prima riga tiene il numero della stazione, la seconda l'autonomia
                int percorso[2][numNodi];
                percorso[0][0] = matrice[0][0]; //Il percorso inizia con la partenza
                percorso[1][0] = matrice[1][0];
                for (int i = 1; i < numNodi; i++) { //Inizializzo le altre celle a 0
                    percorso[0][i] = 0;
                    percorso[1][i] = 0;
                }

                //Altre inizializzazioni
                int indiceInCuiInserire = 1; //Indice in cui inserire la stazione attuale
                int srag_ss = partenza;
                int srag_dalla_prec = partenza;
                for(int k=1; k<numNodi; k++) { //Calcolo l'indice della staz raggiungibile da quella attuale
                    if(matrice[0][0] <= matrice[0][k] + matrice[1][0]) { //Se la staz curr è raggiungibile dalla partenza
                        srag_ss = matrice[0][k]; //Allora aggiorno la stazione raggiungibile
                        srag_dalla_prec = matrice[0][k];
                    }
                    else
                        break;
                }

                //printf("part=%d, aut=%d, srag=%d\n", matrice[0][0], matrice[1][0], srag_dalla_prec);

                if(srag_dalla_prec == arrivo) { //Se posso rag l'arrivo dalla partenza, metto l'arrivo nel percorso e ciao
                    percorso[0][1] = matrice[0][numNodi-1];
                    percorso[1][1] = matrice[1][numNodi-1];
                }
                else if(srag_dalla_prec == partenza) { //Se dalla prec non posso raggiungere nessuna stazione ciao
                }
                else {
                    //Calcolo del percorso: leggo la matrice e vedo se mettere la stazione attuale nel percorso
                    for(int i=1; i<numNodi; i++) {
                        //Stampa percorso per debug
                        /*printf("-----------\n");
                        printf("Percorso attuale:");
                        for (int j=0; j<numNodi; j++) {
                            if(percorso[0][j] != 0)
                                printf(" %d", percorso[0][j]);
                            else {
                                printf(" %d\n", percorso[0][j]);
                                break;
                            }
                        }*/

                        //Calcolo dell'indice della stazione raggiungibile dalla attuale
                        int srag_sa = partenza;
                        if(i!=numNodi-1) { //Se la stazione attuale non è l'arrivo
                            for(int k=i+1; k<numNodi; k++) { //Calcolo l'indice della staz raggiungibile da quella attuale
                                if(matrice[0][i] <= matrice[0][k] + matrice[1][i]) { //Se la staz curr è raggiungibile dalla partenza
                                    srag_sa = matrice[0][k]; //Allora aggiorno la stazione raggiungibile
                                }
                                else
                                    break;
                            }
                        }
                        if(i == numNodi-2) { //Se la stazione attuale è quella prima dell'arrivo
                            //printf("--3--\n");
                            if(matrice[0][i] > matrice[0][numNodi-1] + matrice[1][i]) { //Se da essa posso raggiungere l'arrivo
                                srag_sa = arrivo;
                            }
                        }
                        if(i == numNodi-1) {
                            srag_sa = arrivo;
                        }

                        //printf("\tsa=%d, aa=%d, srag_sa=%d,\n\tsrag_ss=%d, srag_dalla_prec=%d, indice_in_cui_ins=%d\n", matrice[0][i], matrice[1][i], srag_sa, srag_ss, srag_dalla_prec, indiceInCuiInserire);

                        //Aggiunta o eliminazione della stazione attuale
                        if(matrice[0][i] >= srag_dalla_prec) { //Se posso raggiungere la staz attuale dalla prec inserita
                            //printf("--OO--\n");
                            //Devo vedere se tenere la attuale
                            if(indiceInCuiInserire == 1 && percorso[0][1] == 0) {
                                //printf("--A--\n");
                                percorso[0][indiceInCuiInserire] = matrice[0][i];
                                percorso[1][indiceInCuiInserire] = matrice[1][i];
                                srag_ss = srag_sa;

                                if(matrice[0][i+1] < srag_dalla_prec) { //Se non posso raggiungere la stazione succ aggiorno
                                    //printf("--new diff--\n");
                                    indiceInCuiInserire++;
                                    srag_dalla_prec = srag_ss;
                                    //printf("\tsa=%d, aa=%d, srag_sa=%d,\n\tsrag_ss=%d, srag_dalla_prec=%d, indice_in_cui_ins=%d\n", matrice[0][i], matrice[1][i], srag_sa, srag_ss, srag_dalla_prec, indiceInCuiInserire);
                                    if(matrice[0][i+1] < srag_dalla_prec) { //Se ancora non posso raggiungere la succ ciao
                                        break;
                                    }
                                }
                            }
                            else {
                                if(percorso[0][indiceInCuiInserire] == 0) {
                                    //printf("--B--\n");
                                    percorso[0][indiceInCuiInserire] = matrice[0][i];
                                    percorso[1][indiceInCuiInserire] = matrice[1][i];
                                    srag_ss = srag_sa;
                                }
                                else if(srag_sa <= srag_ss) {
                                    //Se dalla attuale posso raggiungere una stazione lontana almeno quanto quella raggiungibile
                                    // dalla staz aggiunta, allora scambio quella scelta con la attuale
                                    //printf("--D--\n");
                                    percorso[0][indiceInCuiInserire] = matrice[0][i];
                                    percorso[1][indiceInCuiInserire] = matrice[1][i];
                                    srag_ss = srag_sa;
                                }
                                if(matrice[0][i] > srag_dalla_prec) {}
                                else if(matrice[0][i+1] < srag_dalla_prec) { //Se non posso raggiungere la stazione succ aggiorno
                                    //printf("--new diff 2--\n");
                                    indiceInCuiInserire++;
                                    srag_dalla_prec = srag_ss;
                                    //printf("\tsa=%d, aa=%d, srag_sa=%d,\n\tsrag_ss=%d, srag_dalla_prec=%d, indice_in_cui_ins=%d\n", matrice[0][i], matrice[1][i], srag_sa, srag_ss, srag_dalla_prec, indiceInCuiInserire);
                                    if(matrice[0][i+1] < srag_dalla_prec) { //Se ancora non posso raggiungere la succ ciao
                                        break;
                                    }
                                }
                            }
                            //printf("--A--\n");

                        }

                        if(matrice[0][i] == arrivo) { //Se la stazione attuale è l'arrivo
                            //printf("--X--\n");
                            /*printf("\tsa=%d, aa=%d, srag_sa=%d,\n\tsrag_ss=%d, srag_dalla_prec=%d\n",
                                   matrice[0][i], matrice[1][i], srag_sa, srag_ss, srag_dalla_prec);
                            printf("Percorso attuale:");
                            for (int j=0; j<numNodi; j++)
                                printf(" %d", percorso[0][j]);
                            printf("\n");*/

                            if(matrice[0][i] >= srag_dalla_prec) { //E lo posso raggiungere dalla nuova prec
                                percorso[0][indiceInCuiInserire] = matrice[0][i];
                                percorso[1][indiceInCuiInserire] = matrice[1][i];
                                break;
                            }
                        }

                        /*printf("Percorso attuale:");
                        for (int j=0; j<numNodi; j++) {
                            if(percorso[0][j] != 0)
                                printf(" %d", percorso[0][j]);
                            else {
                                printf(" %d\n", percorso[0][j]);
                                break;
                            }
                        }*/
                    }
                }

                //Controllo di aver trovato il percorso
                int indiceDellArrivo = 0;
                bool partenzaTrovata = false;
                bool arrivoTrovato = false;
                if(arrivo != 0) {
                    for(int i=0; i<numNodi; i++) {
                        if(percorso[0][i] == partenza)
                            partenzaTrovata = true;
                        if(percorso[0][i] == arrivo) {
                            arrivoTrovato = true;
                            indiceDellArrivo = i;
                            //printf("indiceDellArrivo = %d\n", indiceDellArrivo);
                            //printf("arrivo = %d\n", percorso[0][indiceDellArrivo]);
                        }
                    }
                }
                else { //Se l'arrivo è 0
                    for(int i=0; i<numNodi; i++) {
                        if(percorso[0][i] == partenza)
                            partenzaTrovata = true;
                        if(percorso[0][i] == arrivo) {
                            indiceDellArrivo = i;
                            break;
                        }
                    }
                    //Controllo se posso davvero raggiungere 0 dalla staz prec
                    //printf("prec:%d, aut_prec:%d\n", percorso[0][indiceDellArrivo-1], percorso[1][indiceDellArrivo-1]);
                    if(percorso[0][indiceDellArrivo-1] - percorso[1][indiceDellArrivo-1] <= 0) {
                        //printf("here\n");
                        arrivoTrovato = true;
                    }
                }

                //Controllo se devo cambiare delle tappe
                int indiceStazAtt = numNodi-1;
                //int indiceStazPrec = numNodi-1;
                int indiceStazSucc = numNodi-1;
                for(int i=indiceDellArrivo-1; i>0; i--) { //Scorro il percorso al contrario
                    for(int j=0; j<numNodi; j++) { //Cerco gli indici nella matrice
                        /*if(matrice[0][j] == percorso[0][i-1]) { //Se la stazione attuale è uguale a quella prec nel percorso
                            indiceStazPrec = j;
                        }*/
                        if(matrice[0][j] == percorso[0][i]) { //Se la stazione attuale è uguale a quella attuale nel percorso
                            indiceStazAtt = j;
                        }
                        if(matrice[0][j] == percorso[0][i+1]) { //Se la stazione attuale è uguale a quella succ nel percorso
                            indiceStazSucc = j;
                            break;
                        }
                    }

                    /*printf("Percorso attuale:");
                    for (int j=0; j<numNodi; j++) {
                        if(percorso[0][j] != 0)
                            printf(" %d", percorso[0][j]);
                        else {
                            printf(" %d\n", percorso[0][j]);
                            break;
                        }
                    }*/

                    //printf("StazAtt nel percorso: %d, StazSucc nel percorso: %d, StazPrec nel percorso: %d\n", percorso[0][i], percorso[0][i+1], percorso[0][i-1]);
                    //printf("StazAtt nella mat: %d, StazSucc nella mat: %d\n", matrice[0][indiceStazAtt], matrice[0][indiceStazSucc]);

                    for(int j=indiceStazSucc; j>indiceStazAtt; j--) { //Cerco nella matrice una staz migliore
                        //Posso raggiungere la stazione attuale dalla prec nel percorso?
                        //Posso raggiungere la succ nel percorso dalla stazione attuale?
                        //La stazione attuale è maggiore della succ nel percorso?
                        //Se sono vere tutte, sostituisco percorso[0][i] con la stazione attuale

                        //printf("--R-- StazAtt: %d\n", matrice[0][j]);
                        if(matrice[0][j] <= percorso[0][i+1] + matrice[1][j]) { //Se dalla attuale posso raggiungere la succ nel percorso
                            //printf("--S--\n");
                            if(percorso[0][i-1] <= matrice[0][j] + percorso[1][i-1]) { //Se dalla prec nel percorso posso raggiungere la attuale
                                //printf("--T--\n");
                                percorso[0][i] = matrice[0][j];
                                percorso[1][i] = matrice[1][j];
                                break;
                            }
                        }
                    }

                    /*printf("Percorso attuale:");
                    for (int j=0; j<numNodi; j++) {
                        if(percorso[0][j] != 0)
                            printf(" %d", percorso[0][j]);
                        else {
                            printf(" %d\n", percorso[0][j]);
                            break;
                        }
                    }
                    printf("----\n");*/
                }

                //Stampo il percorso
                if(partenzaTrovata && arrivoTrovato) {
                    //printf("Percorso trovato: ");
                    printf("%d", percorso[0][0]);
                    for(int i=1; i<numNodi; i++) {
                        if(percorso[0][i] != 0)
                            printf(" %d", percorso[0][i]);
                        else if(arrivo == 0) {
                            printf(" 0\n");
                            break;
                        }
                        else {
                            printf("\n");
                            break;
                        }
                    }
                }
                else {
                    printf("nessun percorso\n");
                }
            }
        }
    }
}

//MAIN
int main() {
    //FILE *in = stdin;
    char comando[20];
    puntatore_albero radice = NULL;

    //Leggo comando
    while(scanf("%s", comando) != EOF) {
        if(strcmp("aggiungi-stazione", comando) == 0) {
            radice = aggiungiStazione(radice);
        }
        else if(strcmp("demolisci-stazione", comando) == 0) {
            radice = demolisciStazione(radice);
        }
        else if(strcmp("aggiungi-auto", comando) == 0) {
            aggiungiAuto(radice);
        }
        else if(strcmp("rottama-auto", comando) == 0) {
            rottamaAuto(radice);
        }
        else if(strcmp("pianifica-percorso", comando) == 0) {
            pianificaPercorso(radice);
        }
        //stampaAlbero(radice);
        //printf("------------------------\n");
    }

    //stampaAlbero(radice);
    return 0;
}
