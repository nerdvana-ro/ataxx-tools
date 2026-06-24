# Arbitru și exemplu de agent pentru jocul Ataxx

Acest set de unelte vă permite să creați un program (agent) pentru jocul Ataxx și să organizați partide contra altor programe.

## Agenții

[Agenții](https://github.com/nerdvana-ro/ataxx-tools/tree/main/agent) sînt programe care joacă Ataxx. Agenții citesc starea jocului (configurația tablei și timpul rămas) și tipăresc o mutare.

Repoul include cîțiva agenți, dintre care [Doofus](https://github.com/nerdvana-ro/ataxx-tools/tree/main/agent/doofus) și [Simpleton](https://github.com/nerdvana-ro/ataxx-tools/tree/main/agent/simpleton)joacă aleatoriu (dar corect). Restul agenților (Error, Hang etc.) se comportă anormal și au rolul de a testa buna funcționare a arbitrului.

## Arbitrul

[Arbitrul](https://github.com/nerdvana-ro/ataxx-tools/tree/main/arbiter) organizează turnee între mai mulți agenți. Arbitrul ține evidența fiecărei partide și invocă pe rînd fiecare agent, comunicîndu-i starea curentă. Apoi citește răspunsul agentului și actualizează starea jocului. La finalul fiecărei partide, arbitrul actualizează și raportează clasamentul la zi.

## Pași de urmat

Vă recomand să încercați uneltele în această ordine:

* **Testați arbitrul**: Faceți instalările și configurările necesare pentru a organiza o partidă între două copii ale agentului Doofus.
  * Dacă rulați Windows, va fi nevoie să instalați WSL. Și acest pas este documentat în secțiunea despre arbitru.
* **Învățați strategia jocului:** Ataxx este un joc cu reguli simple, dar strategie complexă. Vă recomand să jucați cel puțin 5 partide între voi, ca să descoperiți ce merge și ce nu merge. Desigur, puteți citi și opinii de pe Internet.
* **Scrieți un client:** De aceea ne-am adunat aici! 😉 Urmați specificațiile din [protocol.md](https://github.com/nerdvana-ro/ataxx-tools/blob/main/protocol.md).

Scopul principal al acestei săptămîni este să scrieți un agent care să bată convingător și constant agentul Doofus.

Desigur, vom avea și un turneu final (vom decide formatul miercuri sau joi). Dacă vă clasați onorabil în acel turneu, cu atît mai bine!

## Cîteva cuvinte despre Doofus

Doofus funcționează astfel:

* Citește intrarea și construiește tabla.
* Generează toate mutările posibile.
* Alege și tipărește una la întîmplare.

Organizarea codului este rezonabil de bună și include clase separate pentru logging, tablă, generatorul de mutări etc. Dacă vă ajută la ceva, puteți să vă inspirați din el pentru organizarea programului vostru.

Generatorul de mutări este eficient (cu bitboards, despre care vom vorbi luni sau marți).

