# Statut de playtest — Pokémon Tactica

## Candidate owner

**Aucune candidate owner actuelle ne doit être considérée comme représentative de l’état courant.**

L’ancienne candidate `31fe7d7c9d33dcc757fede543db249651f7dc53f` reste un témoin historique du playtest effectué jusqu’à Mortimer, mais elle précède notamment :

- la migration canonique des évolutions / Fil Liaison ;
- la régénération du Pokédex évolution ;
- les correctifs de légalité boss ;
- la PR #48 sur l’introduction Tactica, HARD/Recommended et le message du second starter.
- l’intégration des interfaces Épée/Bouclier, HGSS et Noir/Blanc, du sélecteur Difficulty et des garde-fous Méga ;
- le centrage du libellé `TACTICA` dans sa bulle sur l'écran titre.

Le dernier `integration/v1` doit être reconstruit proprement avant tout nouveau playtest. Un simple `git pull` ne met jamais à jour une ROM déjà compilée.

## Correctif prioritaire intégré avant la prochaine candidate

### Rival — progression réintégrée

Le contrat owner est désormais présent côté source canonique, runtime, générateur et tests :

- premier duel : 1 Pokémon niveau 17 ;
- après Hector : **4 Pokémon niveaux 29–32** ;
- Tour Cendrée : **6 Pokémon niveaux 35–38** ;
- Tour Radio : **61–64** ;
- Route Victoire : **65–67** ;
- Mont Sélénite et Plateau : **niveau 95**.

La progression automatisée est couverte ; il reste à observer ces combats dans la prochaine ROM candidate.

## Couverture automatisée actuelle

Les points suivants sont intégrés et protégés automatiquement. Une observation ROM reste utile, mais leur absence de playtest manuel ne justifie pas de les recoder :

- vitesse native x1/x2/x3/x4 ;
- audio indépendant ;
- Shiny Rate ;
- starter/œuf — logique cœur ;
- curseur initial et retour après annulation ;
- second starter distinct du starter principal ;
- évolutions conformes à `EVOLUTIONS.md` ;
- échanges simples → Fil Liaison ;
- échanges + objet → objet officiel tenu + Fil Liaison ;
- cas impossibles validés → Fil Liaison ;
- absence d’anciens `EVO_TRADE` actifs ;
- Pokédex évolution régénéré depuis le runtime et protégé par `sync_tactica_species_evolutions.py --check` ;
- rosters Rocket et règles Méga ;
- caps Rival synchronisés sur le prochain jalon majeur et niveaux Rocket calculés sur le dernier jalon pertinent +2 ;
- talents canoniques du Rival lorsqu’ils sont légaux pour la forme envoyée ;
- Mega Ring placé après Mortimer ;
- premier accès réel des encounters ;
- 405 tables standard synchronisées ;
- Scorplane Route 34 après badge 2 ;
- Wattouat Route 31 ;
- unicité globale des Méga ;
- exactement une Méga par Champion à partir de Mortimer ;
- Jeannine : Aéromite reste l’ace, Méga-Kravarech remplace Gaulet ;
- progression Kanto 75 → 80 → 85 → 90 → 95 → 100 ;
- match retour du Maître niveau 100 ;
- Boss & Conseils et assets du Guide générés depuis les sources canoniques ;
- PR #48 : intro Chen/Tactica, avertissements HARD/Recommended/Custom et refus du second starter identique.

## Évolutions — migration terminée

La migration définie dans [EVOLUTIONS.md](EVOLUTIONS.md) est terminée côté runtime et données Pokédex.

Témoins utiles lors du prochain playtest :

- échange simple → Fil Liaison ;
- échange + objet → objet officiel tenu + Fil Liaison ;
- Élektek → Élekable = Électriseur tenu + Fil Liaison ;
- Rhinoféros → Rhinastoc = Protecteur tenu + Fil Liaison ;
- Téraclope → Noctunoir = Tissu Fauche tenu + Fil Liaison ;
- Mélancolux → Lugulabre = Pierre Nuit uniquement ;
- Lampéroie → Ohmassacre = Pierre Foudre uniquement ;
- mécanique impossible validée → Fil Liaison ;
- évolution officielle par pierre → aucune contrainte de niveau artificielle.

Le Pokédex n’est plus une source manuelle pour ces méthodes : ses champs d’évolution sont générés depuis les fichiers runtime.

## Témoins ROM encore utiles

### Introduction / starter

- intro Chen = texte Pokémon Tactica, sans ancien speech vanilla résiduel ;
- avertissement HARD lisible ;
- RECOMMENDED / CUSTOM compréhensibles ;
- second starter identique → message dédié puis retour au flow existant ;
- curseur starter et annulation à revalider comme témoins de non-régression.

### Difficulty

- option visible ;
- sélection utilisable ;
- persistance après sauvegarde/rechargement ;
- ne pas toucher à Vitesse, Audio ou Shiny Rate sans défaut reproduit.

### Rival / Rocket

- premier rival : un Pokémon, niveau 17, stade légal ;
- après Hector : 4 Pokémon, plage 29–32 ;
- Tour Cendrée : 6 Pokémon, plage 35–38 ;
- Tour Radio : 61–64 ; Route Victoire : 65–67 ; Mont Sélénite/Plateau : 95 ;
- Rocket : Proton 3→6, Petrel/Ariana 4→6, Archer 6 ; niveaux dynamiques +2 selon les jalons réels ;
- vérifier en combat que les talents attendus du Rival se déclenchent selon les archétypes.

### Méga

- Mortimer déclenche réellement sa Méga ;
- Mega Ring reçu après badge 4 ;
- joueur + pierre compatible → commande Méga disponible ;
- forme et talent avant/après transformation cohérents ;
- Jeannine : Kravarech entre sous sa forme/talent de base puis Méga-évolue en Adaptabilité.

### Kanto et deuxième Ligue

- caps 75 / 80 / 85 / 90 / 95 / 100 ;
- Morgane, Erika et Jeannine partagent bien le cap 80 ;
- équipe niveau 100 du match retour du Maître et six objets distincts.

### Encounters

- Route 31 jour : Wattouat ;
- Route 36 : pas de Scorplane/Scorvol avant le badge 2 ;
- Route 34 jour : Scorplane niveaux 25–28 ;
- une méthode tardive sur une ancienne zone ne rehausse pas ses niveaux.

### UI

- équipe Épée/Bouclier : vérifier les six emplacements, les états vide/œuf/statut et toutes les actions du menu contextuel ;
- sac Épée/Bouclier : vérifier les poches, la description, les quantités et l’utilisation d’un objet sur le terrain ;
- Summary Épée/Bouclier : parcourir toutes les pages, dont IV/EV, et vérifier textes, icônes, capacités et navigation ;
- Pokédex HGSS : vérifier la liste, la fiche, les formes et les compteurs ;
- boutique native habillée B2W2 : confirmer l’ouverture sans écran noir ni crash, puis vérifier achat, vente, quantités, argent, description et sortie ;
- HUD Noir/Blanc d’origine : confirmer le retour des décors propres à l’environnement, puis vérifier cadres allié/adversaire, menus action/capacités, statut, shiny, doubles et barre EXP ;
- changement de Pokémon en combat : ouvrir l’équipe, changer de Pokémon et confirmer le retour au combat sans écran d’erreur mémoire ;
- Méga-Évolution : après Mortimer, équiper une Méga-Gemme compatible puis appuyer sur `START` depuis le choix des capacités ; le logo doit changer d’état et la transformation doit précéder l’attaque ;
- aide des capacités : vérifier que `R` affiche les informations sans entrer en conflit avec la commande Méga ;
- menu Start plein écran HGSS/BW : l’ouvrir et le fermer plusieurs fois pour confirmer l’absence d’écran d’erreur et de police corrompue, puis vérifier les six accès, les icônes d’équipe, les retours depuis chaque écran et la sauvegarde ; vérifier aussi le repli compact dans les contextes spéciaux ;
- carte Dresseur Épée/Bouclier : vérifier les deux faces, le portrait, les textes localisés et l’affichage des 16 badges ; vérifier également le header Options, dont `B SAVE & EXIT` ;
- libellé `TACTICA` visuellement centré dans sa bulle sur l'écran titre.

## IA HARD — correctifs ciblés issus du premier playtest

Le premier playtest owner a reproduit les boucles de setup sur Évoli et Mimiqui. La candidate suivante doit confirmer :

- Évoli utilise deux Reflets puis Relais ; un troisième Reflet n’est permis qu’à PV pleins face à un adversaire non Combat, puis Relais devient prioritaire ;
- le relais choisit un receveur pertinent dans l’équipe de Blanche, notamment Ursaring ou Écrémeuh selon le matchup ;
- Mimiqui utilise une Danse-Lames puis attaque au lieu de continuer jusqu’à +6 ;
- les autres setup offensifs convertissent leur avantage en attaque après deux niveaux positifs.

Les témoins Provoc de Cornèbre et Téraclope restent à observer sans autre changement tant qu’aucun défaut courant n’est reproduit.

## Protocole candidate

```bash
git fetch origin
git switch integration/v1
git pull --ff-only origin integration/v1
git rev-parse HEAD
make clean
make hns -j4
```

Avant de lancer mGBA, vérifier que `pokehns.gba` vient réellement d’être régénérée après le pull.

Le compte rendu de playtest doit toujours noter le SHA exact de la ROM testée et uniquement les comportements réellement observés.
