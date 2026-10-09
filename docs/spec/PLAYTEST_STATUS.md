# Statut de playtest — Pokémon Tactica

## Candidate owner

La nouvelle candidate est construite depuis `integration/v1`, commit `e27999c276fc4c2d4898b4afbd2c12f1dbd6f787`. Elle comprend les corrections terrain/climat HARD, CS et dialogues/options du dernier playtest. Son téléchargement est référencé dans [Jouer](../FR/Jouer.md) ; le manifeste du ZIP donne le SHA source et l’empreinte de la ROM exacte.

Cette candidate attend la validation owner des corrections visuelles et comportementales, puis le parcours jusqu’à Mortimer et l’aventure complète. Elle n’est pas une version stable validée. Les anciennes candidates ne contiennent pas ces corrections.

## Correctif prioritaire intégré avant la prochaine candidate

### Rival — progression réintégrée

Le contrat owner est désormais présent côté source canonique, runtime, générateur et tests :

- premier duel : 1 Pokémon niveau 18 ;
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
- caps Rival synchronisés sur le prochain jalon majeur et niveaux Rocket calculés sur la plage de la dernière arène vaincue +2, sans cumul entre admins ;
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

- premier rival : un Pokémon, niveau 18, stade légal ;
- après Hector : 4 Pokémon, plage 29–32 ;
- Tour Cendrée : 6 Pokémon, plage 35–38 ;
- Tour Radio : 61–64 ; Route Victoire : 65–67 ; Mont Sélénite/Plateau : 95 ;
- Rocket : Proton 3→6, Petrel/Ariana 4→6, Archer 6 ; plage de la dernière arène vaincue +2, sans cumul entre admins ;
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

## Contrôles owner des corrections de cohérence

À effectuer sur une ROM construite depuis `integration/v1`, en notant son SHA exact. Les tests automatiques de données et de combat ne remplacent pas ces parcours :

- nouveau départ : recevoir une Potion et dix Poké Balls de l'assistant d'Orme, puis vérifier qu'il n'en redonne pas au retour ;
- Blanche (29–32) puis un admin : équipe Rocket 31, ace 34 ; deux admins sans nouvelle arène gardent cette plage ; le cap correspond au boss courant et ne redescend pas si un cap supérieur est déjà acquis ;
- Mortimer → Chuck → Jasmine → repaire Rocket : caps de préparation cohérents avec le Guide ;
- deuxième Ligue → Red : cap 100 conservé en mode normal et strict, avec sauvegarde/rechargement ;
- Rival Eau : Marshtomp au niveau 18, Laggron aux rencontres dont le niveau permet son évolution ; profil physique et nature Rigide conservés, Méga après badge 4 ; vérifier aussi les autres archétypes ;
- Rival HARD : IV 31, EV du profil et IA renforcée ; NORMAL garde les mêmes espèces, niveaux, capacités et objets ;
- zone dont la plage traverse un seuil : Tynamo 37–38 / Lampéroie 39–40 ; les formes par objet/Fil Liaison restent à faire évoluer par le joueur ; vérifier aussi Headbutt et Safari ;
- branchages conditionnels : Chenipotte selon personnalité, Apitrini selon sexe, Amagara selon horaire. Le wiki affiche les formes possibles dans les plages.

Rectification owner du 9 octobre 2026 : le cas théorique de sac et PC d'objets pleins lors des récompenses initiales n'est pas un défaut de progression établi. À ce stade, le sac est vide et les Poké Balls ne sont pas encore distribuées. Aucun parcours accessible démontrant une perte de récompense n'a été reproduit ; ce point est retiré des défauts confirmés et des corrections à prévoir.

## Corrections du playtest — terrain/climat, CS et dialogues

Les tests mGBA du lot courant passent : 16 groupes de combat terrain/climat, 3 groupes CS, rendu de tous les choix des deux menus (actif/inactif) et déplacement/effacement du curseur Oui/Non dans les trois variantes de fenêtre. Les 32 groupes du filtre `Tactica` passent, y compris les régressions rencontres, boutiques et équilibrage. Les régressions Settings (5), Family (34), Relais/setup (3) et caps (18) passent aussi : 92 groupes mGBA au total, ainsi que 4 tests Python et les validateurs de spec, données et interfaces. La compilation HNS réussit. Ces tests ne constituent pas une validation visuelle de l'éclosion ou un playthrough complet.

À observer dans la nouvelle candidate :

- Wattapik transmet le terrain actif à un attaquant pertinent ; Salarsen reste poseur manuel de secours avec Champ Électrifié. Une attaque super efficace sans KO ne bloque pas le relais ;
- terrain remplacé/expiré : retour d'un poseur avant un nouvel attaquant, puis restauration et relais ; vérifier aussi après KO et sur les cinq autres archétypes ;
- absence de switch impossible sous piège et de sacrifice immédiat sur hazards connus ; ne pas confondre ces limites avec une lecture de capacités cachées ;
- Coupe, Flash, Éclate-Roc, Force, Surf, Vol, Plongée et Cascade avant/après leur condition scénario, avec une équipe incompatible et sans CS apprise ou objet CS ;
- tous les onglets Options et Settings, toutes les valeurs et lignes désactivées, cadres et challenge de type compris ; vérifier header, scroll, sauvegarde et rechargement ;
- éclosion, message du Pokémon éclos, choix de surnom Oui/Non puis retour au terrain ;
- Oui/Non de changement en combat : curseur sur chaque réponse, annulation et répétitions, sans rectangles noirs ni lettres effacées.

Les combats mystère restent planifiés hors V1 ; aucune rencontre supplémentaire n'est ajoutée à cette candidate.

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
