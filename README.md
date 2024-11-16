# Progetto di API 2023
Progetto del corso di Algoritmi e Principi dell'Informatica del Politecnico di Milano (A.A. 2022-2023).

Valutazione ottenuta: 30L/30. Il progetto è consultabile nel file `autostrada.c`.

## Testo del problema
Si consideri un’autostrada descritta come una sequenza di stazioni di servizio. Ogni stazione di servizio si trova ad una distanza dall’inizio dell’autostrada espressa in chilometri da un numero intero positivo o nullo. Non esistono due stazioni di servizio aventi la stessa distanza: ogni stazione di servizio è quindi univocamente identificata dalla sua distanza dall’inizio dell’autostrada.

Ogni stazione di servizio è dotata di un parco veicoli elettrici a noleggio. Ogni veicolo è contraddistinto dall’autonomia data da una carica delle batterie, espressa in chilometri, da un numero intero positivo. Il parco veicoli di una singola stazione comprende al più $512$ veicoli. Presa a noleggio un’auto da una stazione $s$, è possibile raggiungere tutte quelle la cui distanza da $s$ è minore o uguale all’autonomia dell’automobile.

Un viaggio è identificato da una sequenza di stazioni di servizio in cui il conducente fa tappa. Ha quindi inizio in una stazione di servizio e termina in un’altra, passando per zero o più stazioni intermedie. Si assuma che il conducente non possa tornare indietro durante il viaggio, e noleggi un’auto nuova ogniqualvolta faccia tappa in una stazione di servizio. Quindi, date due tappe consecutive $s$ e $t$, $t$ deve essere sempre più lontana dalla partenza rispetto a $s$, e $t$ deve essere raggiungibile usando uno dei veicoli disponibili in $s$.

L’obiettivo del progetto è il seguente: data una coppia di stazioni, pianificare il percorso con il minor numero di tappe tra di esse. Nel caso siano presenti più percorsi con lo stesso numero minimo di tappe (cioè a pari merito),
deve essere scelto il percorso che predilige le tappe con distanza più breve dall’inizio dell’autostrada. In altre parole, si consideri l’insieme degli $n$ percorsi a pari merito $P = {p_1, p_2, ..., p_n}$ dove ogni percorso è una tupla di $m$ elementi
$p_i = ⟨p_{i,1}, p_{i,2}, ..., p_{i,m}⟩$ che equivalgono alla distanza da inizio autostrada di ogni tappa in ordine di percorrenza.

Deve essere scelto l’unico percorso $p_i$ tale che **non esista** un altro $p_j$ con le stesse $k$ tappe finali precedute da una tappa con distanza minore, cioè $∄j, k : ⟨p_{i,m−k+1}, ..., p_{i,m}⟩ = ⟨p_{j,m−k+1}, ..., p_{j,m}⟩ ∧ p_{j,m−k} < p_{i,m−k}$.

### Formato file in ingresso e stampe attese

Il file testuale in ingresso contiene una sequenza di comandi, uno per riga, con il seguente formato. Tutti i valori interi positivi o nulli sono codificabili in 32 bit.
- `aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n`<br />
  - Aggiunge una stazione, posta alla distanza indicata, dotata di `numero-auto`, dall’autonomia indicata.<br />
  - Ad esempio: `aggiungi-stazione 10 3 100 200 300` aggiunge una stazione a distanza 10 dall’inizio dell’autostrada, con un parco di tre veicoli con autonomia 100, 200 e 300 km rispettivamente. Se esiste già una stazione alla distanza indicata, il comando non fa nulla.<br />
  - Stampa attesa come risposta: `aggiunta` oppure `non aggiunta`.<br /><br />
- `demolisci-stazione distanza`<br />
  - Rimuove la stazione posta alla distanza indicata, se essa esiste.<br />
  - Stampa attesa come risposta: `demolita` oppure `non demolita`.<br /><br />
- `aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere`<br />
  - Se la stazione esiste, aggiunge un’auto alla stessa. È possibile avere più auto con la stessa autonomia.<br />
  - Stampa attesa come risposta: `aggiunta` oppure `non aggiunta`.<br /><br />
- `rottama-auto distanza-stazione autonomia-auto-da-rottamare`<br />
  - Rimuove un’automobile dalla stazione indicata, se la stazione esiste ed è dotata di almeno un’automobile con l’autonomia indicata.<br />
  - Stampa attesa come risposta: `rottamata` oppure `non rottamata`.<br /><br />
- `pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo`<br />
  - Richiede di pianificare il percorso con i vincoli sopra indicati. <br />
  - Stampa attesa come risposta: le tappe in ordine di percorrenza, rappresentate con la distanza delle stazioni da inizio autostrada, separate da spazi e alla fine seguite da un a-capo. Devono essere incluse partenza e arrivo; se coincidono la stazione viene stampata una sola volta. Se il percorso non esiste, stampa `nessun percorso`. L’azione di pianificazione non altera le stazioni o il loro parco veicoli. Le stazioni date sono sicuramente presenti.

## Implementazione
### Funzioni implementate
  - [x] `aggiungi-stazione`
  - [x] `demolisci-stazione`
  - [x] `aggiungi-auto`
  - [x] `rottama-auto`
  - [x] `pianifica-percorso` con `partenza < arrivo`
  - [x] `pianifica-percorso` con `partenza > arrivo`

### Strutture dati utilizzate
- Binary Search Tree per memorizzare le stazioni
- Liste per memorizzare le autonomie delle automobili di una stazione
- Matrice per salvare le stazioni su cui calcolare il percorso
- Matrice per memorizzare il percorso

## Voto finale
30L/30: il progetto viene eseguito in `0,464 s` utilizzando `38,0 MiB` di memoria.

### Limiti per i voti
Tutti i test del verificatore utilizzano lo stesso file in input, che deve essere eseguito sempre con meno menoria e tempo, a seconda di quanto alto si vuole il voto.<br />
I limiti di tempo e memoria per i vari test erano:

| NOME TEST | LIMITE DI TEMPO | LIMITE DI MEMORIA |
| ---| --- | --- |
| Open | 40,00 secondi | 2,0 GiB |
| 18 | 19,00 secondi | 128 MiB  |
| 21 | 15,00 secondi | 118 MiB |
| 24 | 10,00 secondi | 108 MiB |
| 27 | 6,00 secondi | 98 MiB |
| 30 | 4,00 secondi | 88 MiB |
| 30L | 1,00 secondo | 78 MiB |
