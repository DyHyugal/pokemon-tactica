# Interface, localisation et wiki joueur

## Baseline actuelle

La baseline wiki courante est celle fusionnée par la PR #29. Elle inclut les retours owner récents et la rubrique **Routes et Villes / Routes & Cities**.

Ne pas restaurer une ancienne passe wiki et ne pas refaire le style global sans demande explicite. Une correction data ou fonctionnelle doit préserver cette baseline.

Direction visuelle : noir / rouge / gris, lisible sur desktop et mobile.

## Langues

FR : noms officiels français des Pokémon, attaques, talents, objets, types et personnages.

EN : termes officiels anglais.

Le contenu joueur doit rester miroir entre les langues. Éviter tout mélange anglais résiduel en FR lorsqu’un nom officiel existe.

La ROM V1 testable reste anglaise ; une ROM française distincte reste postérieure à la V1.

## Navigation

Rubriques joueur :

- Accueil / Home
- Guide
- Changements / Changes
- Pokédex
- Localisations / Locations
- Routes et Villes / Routes & Cities
- Boss & Conseils / Bosses & Tips
- Crédits
- Roadmap

Le switch FR/EN doit conserver la rubrique courante lorsque l’équivalent existe.

## Localisations : source canonique

`data/spec/encounters_standard.json` est la source canonique des 405 tables standard.

`tools/sync_tactica_localization.py` doit maintenir :

- les niveaux FR/EN ;
- les espèces FR/EN ;
- les quatre taux `30/30/30/10` ;
- les lignes Markdown source et docs ;
- les lignes HTML ;
- les index `data-search` bilingues ;
- les compteurs de tables du Pokédex.

Le générateur doit être idempotent et son mode `--check` doit échouer dès qu’une page diverge des données canoniques.

Ne jamais corriger manuellement des centaines de lignes si elles peuvent être régénérées.

Les 4 tables Headbutt et 53 pools Safari restent décrits dans `encounters_special.json`. Leur contenu textuel localisé actuel est conservé tant que ce dataset spécial n’est pas modifié.

## Pokédex joueur

Le Pokédex est une référence strictement liée à la ROM Tactica.

Chaque fiche doit pouvoir exposer :

- lignée évolutive ;
- conditions d’évolution réellement compilées ;
- statistiques de base ;
- type(s) ;
- talents disponibles ;
- attaques par niveau ;
- compatibilités utiles du PNJ moves/CT ;
- localisations réellement disponibles ;
- modifications spécifiques Tactica.

Le lien **Voir les localisations / See locations** reste dédié à la page Localisations.

Cliquer sur le reste de la carte ouvre la fiche détaillée dans la page, avec une UX compatible mobile.

Ne pas importer d’informations anime/cartes/autres jeux. Les wikis externes servent uniquement de référence ergonomique.

Les compteurs de tables affichés sur les cartes et dans les tableaux Pokédex sont générés depuis les rencontres actuelles ; un Pokémon non rencontré directement peut donc afficher `0`.

## Guide

Le Guide décrit la progression joueur sans devenir un walkthrough tour par tour.

État courant à préserver :

- introduction claire sur le remake 2G ;
- starters regroupés par type + Évoli ;
- second starter choisi au laboratoire d’Orme au retour de chez M. Pokémon, après la séquence vol/police ;
- Kanto développé ;
- illustrations/cartes déjà intégrées par la baseline ;
- pas de commentaire de bas de page inutile destiné aux joueurs « perdus ».

## Changements

La page Changements est orientée joueur, pas développeur/prompt.

Elle doit notamment couvrir :

- NORMAL/HARD ;
- starters + œuf ;
- vitesse/audio/shiny ;
- Pokémon et learnsets modifiés ;
- encounters ;
- évolutions solo ;
- les trois PNJ de boutique ;
- Méga-Évolution ;
- refonte UI.

## Routes et Villes

Cette rubrique est **intégrée** et fait partie de la baseline.

Elle répond à « que faire / récupérer dans cette zone ? », tandis que Localisations répond à « où capturer ce Pokémon ? ».

Les rencontres affichées par Routes/Villes doivent venir des données Localisations synchronisées, pas d’une seconde copie manuelle.

Les contenus progression, services, objets, cadeaux et connexions doivent rester fondés sur la ROM Tactica.

## Boss & Conseils

Afficher les rosters réellement canoniques, objets, talents, moves et logique stratégique utile, sans imposer une séquence de tours.

Après toute modification de `bosses.json`, régénérer les données wiki concernées si elles sont dérivées de cette source.

## Validation

Après une modification encounters/wiki :

1. corriger la source canonique ;
2. régénérer runtime si nécessaire ;
3. lancer `tools/sync_tactica_localization.py` ;
4. lancer son `--check` ;
5. lancer `tools/validate_tactica_localization.py` ;
6. contrôler visuellement uniquement les pages réellement touchées.

Une passe éditoriale wiki n’autorise pas à réintroduire des données gameplay anciennes.
