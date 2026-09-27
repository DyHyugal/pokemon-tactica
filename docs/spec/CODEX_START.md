# Reprise Codex — Pokémon Tactica V1

## Démarrage obligatoire

Toujours repartir du dernier `integration/v1` :

```bash
git fetch origin
git switch integration/v1
git pull --ff-only origin integration/v1
git rev-parse HEAD
```

Puis lire, dans cet ordre :

1. `AGENTS.md`
2. `docs/spec/SOURCE_OF_TRUTH.md`
3. `docs/spec/FEATURE_STATUS.md`
4. `docs/spec/PLAYTEST_STATUS.md`
5. `docs/spec/EVOLUTIONS.md` dès qu'un sujet touche une évolution, un objet d'évolution, une forme ou le wiki Pokédex
6. les autres documents métier réellement concernés
7. les `data/spec/*.json` concernés

Une branche/PR non fusionnée n’est jamais considérée comme l’état testable owner.

## Règle documentaire

Les docs actives décrivent uniquement l’état courant. Ne pas conserver plusieurs couches « décision initiale / correction / correction finale ». L’historique est dans Git.

Hiérarchie : `SOURCE_OF_TRUTH.md` → docs métier → JSON canoniques → runtime généré.

## Baseline à protéger

Ne pas recoder sans défaut démontré :

- vitesse native x1/x2/x3/x4 ;
- audio indépendant ;
- Shiny Rate ;
- système d'évolution solo, sous réserve de la migration canonique imposée par `docs/spec/EVOLUTIONS.md` ; les anciennes adaptations HnS contradictoires ne sont pas protégées ;
- sélection des 30 starters + Évoli ;
- curseur initial du sélecteur et retour après annulation ;
- œuf d’Orme distinct du starter principal ;
- rival fixe par archétype, premier combat niveau 17, progression 1→3→4→6 ;
- rosters Rival/Rocket courants ;
- talents canoniques du Rival générés depuis `rival.json` et appliqués lorsqu’ils sont légaux pour la forme envoyée ;
- Méga Rival après badge 4 ;
- Méga Rocket uniquement FINAL ;
- Archer = Méga-Sharpedo sans Abri préalable ;
- talents pré-Méga légaux ;
- Mega Ring après Mortimer ;
- règle du premier accès réel pour les encounters ;
- Route 36 à 14–17 ;
- 405 tables standard synchronisées ;
- baseline wiki fusionnée via PR #29, y compris Routes/Villes.

## État encounters courant

- quatre slots `30/30/30/10` ;
- Scorplane : Route 34 jour, niveaux 25–28, uniquement après badge 2 ;
- Wattouat : Route 31 jour ;
- les localisations/wiki doivent être régénérés depuis les JSON, espèces **et** niveaux ;
- la couverture du dataset standard pré-Ligue est complète : 479 espèces utilisées sur 479 sont disponibles avant/à la première Ligue. Conserver cette couverture ; ne pas réintroduire d’espèce uniquement post-Ligue lors d’une future passe.

## État boss/Méga courant

- exactement une Méga par Champion à partir de Mortimer ;
- unicité globale Champions / Conseil 4 / Rival / Rocket ;
- Jeannine : Aéromite reste l’ace ; Méga-Kravarech @ Dragalgite remplace Gaulet ;
- forme de base légale avant Méga, talent Méga appliqué par la transformation ;
- Simiabraz utilise Acrobatie chez Chuck et Aldo ; Méga-Altaria possède le bonheur runtime maximal pour Retour, sans exposer cette valeur dans le wiki.

## Progression Kanto courante

- Major Bob 75 ; Morgane, Erika et Jeannine 80 ; Ondine 85 ; Pierre 90 ; Auguste 95 ; Blue 100.
- Deuxième Ligue et match retour du Maître : niveau 100.
- Rival : logique de niveau inchangée.
- Match retour du Maître : Voltali, Méga-Dracaufeu X, Hydragon, Miascarade, Exagide, Carchacrok.

# Reprise prioritaire — playtest owner jusqu’à Mortimer / badge 4

Ce bloc remplace l’ancienne consigne « UI intégrée à confirmer ». Le playtest a été effectué sur une ROM fraîche jusqu’à Mortimer. Les observations ci-dessous sont des résultats runtime réels : **ne pas recoder les points validés** et traiter en priorité les défauts démontrés.

## Ordre d’exécution obligatoire

Traiter les chantiers **séquentiellement et jusqu’au bout**. Ne pas commencer un bloc, partir sur un autre sujet, puis revenir au premier plus tard.

L’ordre de reprise obligatoire est :

### PRIORITÉ 1 — UI/UX réellement visible en ROM

Traiter et valider entièrement ce bloc avant de passer à Difficulty :

- Menu principal Start encore blanc/crème ;
- Summary toujours illisible/superposé ;
- HUD combat encore largement blanc + plaque blanche sous les PV ;
- boutiques/Centre commercial encore blanches et parfois sans contour complet ;
- menu Options : chevauchement du header depuis `B SAVE & EXIT`.

Corriger les vrais assets/tilemaps/windows/runtime utilisés, pas seulement satisfaire un validateur statique. Le bloc UI/UX n’est considéré terminé qu’après correction des défauts identifiés, tests pertinents et mise à jour des statuts.

### PRIORITÉ 2 — Difficulty

Traiter et valider entièrement ce bloc après UI/UX :

- l’option doit être visible, utilisable et persistante ;
- ne pas toucher à Vitesse, Audio ou Shiny Rate, déjà validés.

Ne pas commencer le bloc Méga tant que Difficulty n’est pas terminé.

### PRIORITÉ 3 — Méga-Évolution

Traiter le système Méga **end-to-end dans un seul chantier**, puis le valider avant de passer aux évolutions :

- Mortimer doit réellement Méga-évoluer ;
- le joueur doit pouvoir Méga-évoluer après obtention du Mega Ring avec une pierre compatible ;
- vérifier activation boss, flag/variable du Mega Ring, disponibilité joueur, bouton/HUD, objet tenu, transformation et talents pré/post-Méga ;
- ajouter les non-régressions nécessaires.

Ne pas corriger une moitié du flux Méga puis reporter le reste à plus tard.

### PRIORITÉ 4 — Évolutions

Une fois le bloc Méga entièrement terminé, poursuivre la migration canonique définie par `docs/spec/EVOLUTIONS.md`.

État déjà fusionné à protéger :

- PR #41 : suppression des seuils `IF_MIN_LEVEL` artificiels sur les routes concernées ;
- PR #42 : restauration des méthodes officielles non-échange, nettoyage des raccourcis HnS, Évoli et plusieurs cas spéciaux ;
- migration Fil Liaison : toutes les évolutions par échange ont été converties vers Fil Liaison, avec objet officiel tenu lorsque requis ; les routes `EVO_TRADE` et remplacements HnS associés ne font plus partie du runtime Tactica ;
- PR #40 : correctifs de légalité boss déjà fusionnés, notamment Blanche/Ursaring et Mortimer/Ossatueur d’Alola.

État du bloc évolution :

- méthodes non-échange officielles : traitées ;
- échanges simples et échanges + objet : traités via Fil Liaison ;
- mécaniques impossibles : traitées pour Sépiatop, Dofin, Tutafeh-Galar, Ursaring et Meltan ;
- validateur générique : intégré à la CI ;
- Pokédex/wiki : champs d'évolution régénérés depuis le runtime et verrouillés par un synchroniseur `--check`.

**Le bloc évolution est terminé. Ne pas le rouvrir sans défaut démontré.**

### PRIORITÉS SUIVANTES — seulement après les quatre blocs ci-dessus

- scaling/progression du Rival ;
- texte de refus du second starter déjà possédé ;
- ajustements IA ciblés (Mimiqui, Provoc de Cornèbre, Téraclope) ;
- autres écarts non bloquants.

Le retour wiki suivant est déjà traité par la baseline wiki dédiée et **ne doit pas être repris** :
- portraits dresseurs Boss & Conseils : transparence de l’index palette 0 générée explicitement pour le navigateur, sans fond coloré.

L’ancien point « Élektek → Élekable via Électriseur niveau 36 » reste obsolète. La cible est **Électriseur tenu + Fil Liaison** conformément à `EVOLUTIONS.md`.

Ne pas consommer du temps sur un bloc de priorité inférieure tant que le bloc prioritaire en cours n’est pas terminé et validé.

## 1. UI/UX — défauts confirmés en ROM

La direction reste : **noir / rouge / gris, sans grandes surfaces blanches**.

Défauts observés :

- **Menu principal Start** : grandes surfaces blanc/crème toujours présentes ;
- **Summary** : toujours très illisible/superposé. La précédente modification n’a pas constitué la recomposition structurelle BG/tilemaps/windows attendue ;
- **HUD combat** : plaque blanche toujours visible sous les PV joueur ; panneaux Attaque/Sac/Équipe/Fuite encore très blancs et incohérents avec la charte ;
- **Boutiques / Centre commercial** : UI pratiquement inchangée, grands fonds blancs ; certains menus ont un contour incomplet/manquant ;
- **Menu Options** : depuis l’ajout de `B SAVE & EXIT`, le header se chevauche avec la navigation `L PREVIOUS / FEATURES… / R NEXT`. Recomposer l’en-tête afin que toutes les indications restent lisibles simultanément.

Le validateur UI actuel donne de faux positifs : il vérifie surtout la présence de constantes/assets ciblés mais ne garantit pas le rendu final. Le renforcer sur les vrais assets/tilemaps/windows/runtime concernés. Un contrôle statique ne remplace pas l’observation ROM.

## 2. Difficulty

Validé en ROM :

- vitesse x1/x2/x3/x4 : OK ;
- audio : OK ;
- Shiny Rate : OK.

À corriger :

- l’option **Difficulty est absente du menu Settings**. Restaurer l’entrée de difficulté et vérifier sa persistance. Ne pas modifier les trois réglages déjà validés.

## 3. Méga-Évolution — blocage prioritaire

Défauts confirmés en ROM :

- **Mortimer : Ectoplasma n’a pas Méga-évolué** ;
- après victoire, le **Mega Ring est bien reçu** ;
- pourtant le joueur **ne peut toujours pas Méga-évoluer** : Dracaufeu tient bien sa Méga-Gemme, mais aucune option/commande Méga n’apparaît dans le HUD ;
- aucun crash.

Diagnostiquer le système end-to-end :

- condition d’activation boss ;
- flag/variable du Mega Ring ;
- disponibilité joueur ;
- bouton/HUD ;
- objet tenu compatible ;
- déclenchement effectif en combat ;
- forme/talent avant et après Méga.

Ajouter des tests démontrant au minimum :

1. aucune Méga joueur avant Mega Ring ;
2. Mega Ring débloqué après Mortimer ;
3. Pokémon + pierre compatible → Méga disponible ;
4. Mortimer déclenche réellement Méga-Ectoplasma ;
5. talent pré-Méga puis talent Méga correct.

## 4. Évolutions — migration canonique obligatoire

La validation historique « Élektek → Élekable via Électriseur utilisable au niveau 36 » est **obsolète** et ne protège plus ce runtime.

La source normative est désormais `docs/spec/EVOLUTIONS.md`, basée sur les méthodes officielles 9G documentées par Poképédia.

Règle Tactica :

- **échange simple** → utiliser **Fil Liaison** ;
- **échange + objet tenu** → le Pokémon tient l’objet officiel puis le joueur utilise **Fil Liaison** ;
- **mécanique officielle impossible à reproduire proprement** (ex. Sépiatop, Dofin/Superdofin et cas équivalents validés) → **Fil Liaison** ;
- toute évolution officielle réalisable autrement reste **inchangée** : pas de niveau minimum, pierre ou objet de substitution inventé.

Conséquences déjà validées :

- Élektek → Élekable = **Électriseur tenu + Fil Liaison** ;
- Rhinoféros → Rhinastoc = **Protecteur tenu + Fil Liaison** ;
- Téraclope → Noctunoir = **Tissu Fauche tenu + Fil Liaison** ;
- Mélancolux → Lugulabre = **Pierre Nuit uniquement** ;
- Lampéroie → Ohmassacre = **Pierre Foudre uniquement** ;
- Scalproie → Scalpereur doit revenir à la mécanique officielle 9G si elle est techniquement réalisable ; la route Roche Royale est une adaptation HnS à supprimer.

Audit déjà établi :

- le commit HnS `b7ea13f0ff0088edb8382c2e012e9e511119d6be` a ajouté **125 routes** avec `IF_MIN_LEVEL` ;
- **23** seulement appartiennent réellement au périmètre des évolutions par échange ;
- **102** routes hors échange, touchant **67 Pokémon sources**, ont reçu un seuil artificiel ;
- plusieurs raccourcis d’objet non officiels ont également été identifiés (Scalpereur, Verpom, Pomdramour, Théffroi, Poltchageist, Crèmy, Duralugon, Wushours, Charbambin, Tutafeh-Galar, etc.).

État actuel :

- suppression des seuils `IF_MIN_LEVEL` artificiels : traitée ;
- restauration des méthodes officielles non-échange et suppression des principaux raccourcis HnS : traitée ;
- Évoli et plusieurs cas spéciaux non-échange : traités ;
- échanges simples : migrés vers **Fil Liaison** ;
- échanges + objet : migrés vers **objet officiel tenu + Fil Liaison** ;
- aucune route `EVO_TRADE` ne doit subsister dans le runtime Tactica.

Le prochain travail ne concerne plus les évolutions. Reprendre l'ordre global de priorité défini plus haut sans revenir sur ce bloc sauf régression démontrée.

Ne pas considérer le wiki actuel comme source de vérité pendant cette migration : il doit refléter le runtime corrigé, pas l’inverse.

## 5. Œuf / second starter

Validé en ROM :

- impossible de reprendre exactement le starter principal ;
- réception correcte ;
- flow conforme à la décision précédente.

À corriger uniquement :

- reformuler le dialogue lorsqu’une espèce déjà possédée est reconnue. Intention souhaitée :  
  **« Tu as bien reconnu l’espèce, mais tu la possèdes déjà. Essaie d’en choisir une différente ! »**
- conserver le flow actuel ; ne pas le réécrire.

## 6. Rival — progression/niveaux faux après Hector

Validé :

- avec Salamèche joueur, premier Rival Eau = **Flobio niv.17**, conforme ;
- sets observés cohérents.

À corriger :

- après Hector, le combat observé utilise bien **Goélise / Flobio / Hypotrempe**, mais niveaux **15 / 16 / 18** : incorrect ;
- entre Blanche et Mortimer, le Rival n’a encore que **4 Pokémon** et reste basé sur les niveaux d’Hector : incorrect ;
- vérifier les événements réels avant de modifier la progression : il ne semble pas y avoir de combat Rival juste après le badge 1 ;
- corriger la logique générale de scaling/progression afin que les combats Rival réellement déclenchés utilisent le nombre de Pokémon et les niveaux attendus au point réel de progression, en cohérence avec le cap/prochain boss ;
- corriger source canonique + générateur/runtime + tests, sans hardcoder uniquement les deux combats observés.

## 7. Blanche — corrigé, ne pas reprendre

Défaut confirmé :

- Blanche utilise encore **Teddiursa niv.29**.

Attendu :

- utiliser **Ursaring à partir du niveau légal d’évolution**, donc niveau 30 minimum dans ce combat ;
- aligner source canonique, runtime, wiki et tests.

## 8. IA — conserver la base, améliorer quelques décisions

L’IA est globalement jugée correcte. Ne pas la réécrire entièrement.

Cas observés à améliorer :

- **Mimiqui** : après Disguise cassé et avec gros boost d’Attaque (+2/+4), s’il peut KO avec **Ombre Portée**, la priorité doit être fortement favorisée au lieu d’un move inférieur qui le laisse se faire dépasser ;
- **Cornèbre d’Albert** : a utilisé Provoc contre un Pokémon Électrik offensif sans cible évidente pour Provoc. Améliorer l’évaluation de Provoc afin d’éviter son usage lorsqu’aucune action pertinente n’est réellement bloquable ;
- **Téraclope** : comportement trop passif ; n’a quasiment pas cherché à brûler/contrôler. Auditer set + décision IA. Ne pas remplacer automatiquement Ténèbres par Ball’Ombre sans comparer dégâts/statistiques/légalité.

Préférer une amélioration générique de la fonction de scoring IA lorsqu’elle résout proprement le cas, plutôt qu’un script spécial par Pokémon.

## 9. Points validés en ROM — à consigner, ne pas recoder

- starter + UX du sélecteur : OK ;
- Stalgamin femelle → Momartik lorsque cette évolution est choisie : OK ;
- œuf / deuxième starter distinct + réception : OK ;
- Route 31 : encounters conformes ; Tarsal cohérent avec le slot rare 10 % ;
- Albert : équipe/niveaux conformes ; Cornèbre @ Ceinture Force ; Vent Arrière + Demi-Tour observés ; aucun soin utilisé ;
- PNJ Eau Fraîche de l’arène d’Hector : OK ;
- Proton EARLY : équipe correcte et comportement cohérent, notamment Tadmorv-A utilisant Entrave intelligemment ;
- Hector : Pomdepik joue bien suicide lead PDR → Picots ; Insécateur suffisamment tanky pour être cohérent avec Évoluroc ; soins <= 2 ;
- Route 34 : composition/niveaux conformes, Scorplane au bon endroit ;
- Route 36 : composition/niveaux conformes ;
- PNJ objets/capacités du **Centre Commercial de Doublonville** : fonctionnels. Ne pas supposer qu’ils doivent exister dans chaque Poké Mart si la source de vérité/wiki les place uniquement ici ;
- Greninja possède bien une Méga-Gemme ; **ne pas lancer maintenant un inventaire complet des Méga**, ce sera une passe dédiée ultérieure ;
- Mortimer : équipe globalement correcte, IA plutôt propre, aucun crash ;
- Mega Ring reçu après Mortimer : OK, mais utilisation de la Méga joueur cassée comme décrit plus haut.

## 10. Documentation/statuts à mettre à jour pendant le correctif

À partir de ce playtest réel :

- **Summary ROM** → `[À CORRIGER]` ;
- **HUD combat** → `[À CORRIGER]` ;
- **Menu principal Start** → ajouter explicitement comme `[À CORRIGER]` si absent du registre ;
- **Menu Options / header** → `[À CORRIGER]` ;
- **UI boutiques** → `[À CORRIGER]` ;
- Rival progression/scaling → `[À CORRIGER]` tant que le défaut runtime n’est pas corrigé ;
- Méga boss/joueur → `[À CORRIGER]` tant que Mortimer et le joueur ne déclenchent pas réellement la Méga.

Ne pas laisser ces points sous la formulation « intégré, rendu à confirmer » : le rendu/runtime a maintenant été observé et a échoué.

## Wiki

La passe owner de la PR #29 est la baseline. Ne pas restaurer une ancienne version et ne pas refaire le style global.

Les corrections data doivent passer par les sources canoniques puis les générateurs wiki. Boss & Conseils se régénère avec `tools/sync_tactica_boss_wiki.py` et les images locales avec `tools/sync_tactica_wiki_assets.py`. Le synchroniseur Localisations/Pokédex doit rester idempotent et la recherche HTML doit refléter les espèces canoniques FR/EN.

Boss & Conseils conserve ses cartes repliables, ses sprites de dresseurs/Pokémon et les talents du Rival issus de la source canonique. La page Changements utilise deux colonnes indépendantes pour éviter les grands vides provoqués par la hauteur de la carte des starters.

Les anciennes règles wiki d’évolution (notamment Élektek → Élekable via Électriseur niveau 36) sont **obsolètes** lorsqu’elles contredisent `docs/spec/EVOLUTIONS.md`. Corriger d’abord les sources runtime, puis régénérer le Pokédex/wiki. Les portraits locaux des dresseurs restent générés avec transparence explicite de l’index palette 0.

## Politique de validation

Pendant le développement : tests ciblés du domaine + générateurs `--check`.

Avant merge vers `integration/v1` :

- validateurs pertinents verts ;
- synchronisations `--check` vertes ;
- build/smoke tests si code ou runtime modifié ;
- CI verte.

Le playtest owner final n’est pas requis pour merger une correction automatisée propre. Les vérifications ROM restantes sont consignées comme telles.

Pour cette reprise précise :

- corriger les sources canoniques avant les fichiers générés ;
- ajouter des non-régressions ciblées pour Rival scaling/progression, Méga et **mécanique d’évolution Fil Liaison / conformité à `EVOLUTIONS.md`** ;
- pour l’UI, ne pas considérer le validateur statique comme preuve de réussite visuelle ;
- régénérer ce qui doit l’être ;
- build HnS propre ;
- tests ciblés + CI ;
- branche dédiée + PR vers `integration/v1`.

Avant une candidate owner : CI verte sur le SHA exact, `make clean && make hns -j4`, puis checklist ROM de `PLAYTEST_STATUS.md`.

## Compte rendu attendu

Toujours fournir factuellement :

- SHA de départ et SHA final ;
- branche/PR ;
- fichiers modifiés ;
- causes trouvées ;
- données/générateurs corrigés ;
- tests et CI ;
- synchronisations/idempotence ;
- contrôles ROM encore non effectués.

Ne jamais déclarer un test mGBA effectué s’il ne l’a pas été.
