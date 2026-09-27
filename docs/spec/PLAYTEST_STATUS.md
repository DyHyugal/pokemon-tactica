# Statut de playtest — Pokémon Tactica

## Candidate owner

**Candidate owner courante : gameplay `31fe7d7c9d33dcc757fede543db249651f7dc53f`.**

- fusion dans `integration/v1` : PR #34 ;
- CI d'intégration verte : [Family Remix validation #36308010995](https://github.com/DyHyugal/pokemon-tactica/actions/runs/36308010995) ;
- build propre : `make clean && make hns -j4`, terminé le 27 septembre 2026 à 11:12 CEST ;
- ROM : `pokehns.gba`, 33 554 432 octets ;
- SHA-256 : `EFD0D15B78A64EC9A9E5CE712F0FF7C9B80DD0E6AD04A965B1362BEFADC15FE4`.

Le commit de documentation qui enregistre cette candidate ne modifie ni le gameplay ni la ROM : le SHA de référence du binaire reste `31fe7d7c9d33dcc757fede543db249651f7dc53f`.

**Cette candidate précède le nouveau contrat d’évolution de `EVOLUTIONS.md` et ne valide donc pas la conformité 9G / Fil Liaison.**

Les contrôles mGBA de l'ancienne candidate ne couvrent pas ce bloc et ne sont pas reportés comme effectués.

## Ce qui est déjà couvert automatiquement

Les points suivants ne doivent pas être recodés simplement parce qu’une observation ROM reste souhaitable :

- vitesse native x1/x2/x3/x4 ;
- audio indépendant ;
- Shiny Rate ;
- difficulté NORMAL/HARD : option Settings, HARD par défaut et persistance runtime ;
- starter/œuf — logique cœur ;
- curseur initial et retour après annulation — garde-fous automatiques ;
- premier rival niveau 17 et stade légal ;
- rosters Rival/Rocket et progression ;
- caps Rival synchronisés sur le prochain jalon majeur et niveaux Rocket calculés sur le dernier jalon pertinent +2 ;
- talents canoniques du Rival appliqués lorsqu’ils sont légaux pour la forme envoyée ;
- talents pré-Méga légaux ;
- Mega Ring placé après Mortimer ;
- commande Méga joueur sur START, aide des attaques sur SELECT ;
- Mortimer NORMAL/HARD : Ectoplasmite, transformation Méga et talent Corps Maudit → Marque Ombre couverts par un test de combat ;
- premier accès réel pour les encounters ;
- Route 36 à 14–17 ;
- Scorplane Route 34 jour après badge 2 ;
- Wattouat Route 31 jour ;
- synchronisation runtime des 405 tables ;
- unicité globale des Méga ;
- exactement une Méga par Champion à partir de Mortimer ;
- Jeannine : Aéromite reste l’ace ; Méga-Kravarech @ Dragalgite remplace Gaulet ;
- courbe Kanto 75 → 80 → 85 → 90 → 95 → 100 et groupe central partagé ;
- match retour du Maître niveau 100 : source canonique/runtime/objets uniques ;
- Boss & Conseils généré depuis les sources canoniques ; cartes et sprites du Guide générés ;
- Boss & Conseils : cartes repliables, sprites de dresseurs/Pokémon et talents du Rival générés ;
- Simiabraz de Chuck/Aldo avec Acrobatie et Méga-Altaria à bonheur maximal pour Retour ;
- Localisations/Pokédex dérivés des encounters canoniques.

## Évolutions — migration automatisée terminée

Les observations et tests historiques sur les « évolutions solo » prouvent uniquement que l’ancienne implémentation HnS fonctionnait techniquement. Ils ne valident plus la cible produit.

Le runtime, le validateur et les données Pokédex sont synchronisés. Le prochain playtest peut couvrir :

- échange simple → Fil Liaison ;
- échange + objet → objet officiel tenu + Fil Liaison ;
- mécanique impossible validée → Fil Liaison ;
- évolution par pierre officielle → aucune contrainte de niveau ajoutée ;
- Élektek → Élekable = Électriseur tenu + Fil Liaison ;
- Rhinoféros → Rhinastoc = Protecteur tenu + Fil Liaison ;
- Téraclope → Noctunoir = Tissu Fauche tenu + Fil Liaison ;
- Mélancolux → Lugulabre = Pierre Nuit uniquement ;
- Lampéroie → Ohmassacre = Pierre Foudre uniquement ;
- Pokédex/wiki régénéré et identique au runtime réellement compilé — couvert automatiquement ; vérifier seulement la présentation en navigateur si utile.

## Témoins ROM encore utiles

### Kanto et deuxième Ligue

- vérifier que les caps affichés et appliqués suivent 75 / 80 / 85 / 90 / 95 / 100 ;
- vérifier que Morgane, Erika et Jeannine partagent le cap 80 quel que soit leur ordre ;
- vérifier l’équipe niveau 100 du match retour du Maître et ses six objets distincts ;

### Starter / œuf

- première ouverture : curseur en haut, pas sur `Retour` ;
- annuler un starter : revenir exactement sur ce Pokémon ;
- vérifier Élekid comme ancien cas de régression ;
- œuf d’Orme : impossible de récupérer exactement le starter principal.

### Rival / Rocket / Méga

- premier rival : un Pokémon, niveau 17, stade légal ;
- Rival après Hector : 4 Pokémon, plage 29–32 ; Tour Cendrée : 6 Pokémon, plage 35–38 ;
- progression tardive Rival : Tour Radio 61–64, Route Victoire 65–67, Mont Sélénite/Plateau 95 ;
- Rocket : Proton 3→6, Petrel/Ariana 4→6, Archer 6 ; niveaux dynamiques +2 selon les jalons réels ;
- vérifier en combat que les talents attendus du Rival se déclenchent selon les archétypes ;
- Mortimer : Méga réellement déclenchée sans assert ;
- réception du Mega Ring après badge 4 puis utilisation joueur avant badge 5 ;
- Jeannine : Aéromite reste l’ace ; Kravarech du slot 3 entre avec un talent de base légal puis Méga-évolue en Adaptabilité.

### Encounters

- Route 31 jour : Wattouat présent ;
- Route 36 jour : aucun Scorplane/Scorvol, Mimigal restauré au slot 30 % ;
- Route 34 jour : Scorplane présent après badge 2, niveaux 25–28, slot 30 % ;
- une méthode tardive sur une ancienne zone ne rehausse pas les niveaux.

### UI

- Summary : pages lisibles, aucune superposition/ancienne zone concours parasite, IV/EV exploitables ;
- HUD : aucune plaque blanche résiduelle à gauche de la barre PV ; action/attaques cohérents ;
- boutiques : fond rouge, texte noir, sélection lisible.

## Protocole candidate

```bash
git fetch origin
git switch integration/v1
git pull --ff-only origin integration/v1
git rev-parse HEAD
make clean && make hns -j4
```

Avant de lancer mGBA, vérifier que `pokehns.gba` vient bien d’être régénérée après le pull.

Le playtest doit ensuite noter le SHA exact et uniquement les comportements réellement observés.

Un simple `git pull` ne met pas à jour la ROM existante.
