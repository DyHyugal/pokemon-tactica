# Statut de playtest — Pokémon Tactica

## Candidate owner

**Aucune candidate owner n’est actuellement déclarée.**

Un SHA n’est inscrit ici comme candidate que lorsque toutes les corrections annoncées sont fusionnées dans `integration/v1`, que la CI est verte sur ce SHA et qu’une ROM fraîche a été reconstruite.

## Ce qui est déjà couvert automatiquement

Les points suivants ne doivent pas être recodés simplement parce qu’une observation ROM reste souhaitable :

- vitesse native x1/x2/x3/x4 ;
- audio indépendant ;
- Shiny Rate ;
- évolutions solo ;
- starter/œuf — logique cœur ;
- curseur initial et retour après annulation — garde-fous automatiques ;
- premier rival niveau 17 et stade légal ;
- rosters Rival/Rocket et progression ;
- talents pré-Méga légaux ;
- Mega Ring placé après Mortimer ;
- premier accès réel pour les encounters ;
- Route 36 à 14–17 ;
- Scorplane Route 36 jour ;
- Wattouat Route 31 jour ;
- synchronisation runtime des 405 tables ;
- unicité globale des Méga ;
- exactement une Méga par Champion à partir de Mortimer ;
- Jeannine = Méga-Kravarech @ Dragalgite ;
- Localisations/Pokédex dérivés des encounters canoniques.

## Témoins ROM encore utiles

### Starter / œuf

- première ouverture : curseur en haut, pas sur `Retour` ;
- annuler un starter : revenir exactement sur ce Pokémon ;
- vérifier Élekid comme ancien cas de régression ;
- œuf d’Orme : impossible de récupérer exactement le starter principal.

### Rival / Rocket / Méga

- premier rival : un Pokémon, niveau 17, stade légal ;
- progression des tailles Rival/Rocket ;
- Mortimer : Méga réellement déclenchée sans assert ;
- réception du Mega Ring après badge 4 puis utilisation joueur avant badge 5 ;
- Jeannine : Kravarech entre avec un talent de base légal puis Méga-évolue en Adaptabilité.

### Encounters

- Route 31 jour : Wattouat présent ;
- Route 36 jour : Scorplane présent, niveaux 14–17 ;
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
