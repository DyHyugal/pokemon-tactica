# Interface, traduction et wiki

## Direction visuelle

La direction visuelle custom de Pokémon Tactica est **noir / rouge / gris**. Les interfaces custom ne doivent pas utiliser de grandes surfaces blanches ni un blanc dominant pour les textes/panneaux. Le HUD de combat, le menu principal, le Summary, les fenêtres des PNJ custom et le wiki doivent être harmonisés autour de cette palette tout en gardant un contraste lisible.

Les valeurs CSS et palettes doivent être recoupées avec les assets/code réellement intégrés, puis vérifiées par capture en ROM et dans le navigateur. Les validateurs ne doivent pas figer un ancien rendu qui contredit cette décision owner.

## Traduction

FR : noms officiels français des Pokémon, attaques, talents, objets, types et personnages ; EN : termes officiels anglais. Relire les textes naturels, sans anglais résiduel en FR (par exemple Famignol, pas Maushold) ni jargon inexpliqué. Le contenu doit être un miroir entre langues.

Le moteur de recherche du site doit être localisé :
- FR : index principal sur les noms français, anglais accepté comme alias, et tolérance aux accents si possible ;
- EN : index principal anglais, français accepté comme alias si possible.

Le changement de langue doit conserver **la page équivalente** : Guide ↔ Game Guide, Changements ↔ Changes, Pokédex ↔ Pokedex, Localisations ↔ Locations, Boss & Conseils ↔ Bosses & Tips, Crédits ↔ Credits, Roadmap ↔ Roadmap. Ne pas renvoyer systématiquement à l'accueil.

## Structure et contenu du wiki

Navigation cible : **Guide de jeu**, **Changements**, **Pokédex**, **Localisations**, **Boss & Conseils**, **Crédits**, **Roadmap**. Pokédex et Localisations restent deux rubriques distinctes.

### Guide de jeu

Le Guide doit être un **vrai guide pratique**, pas une introduction marketing. Il explique concrètement :
- NORMAL / HARD et ce qui change réellement ;
- paramètres et sauvegarde ;
- level caps ;
- sélection des starters et second starter via l'œuf ;
- Training NPC ;
- taux shiny ;
- vitesse native ;
- audio ;
- boutiques/PNJ stratégiques ;
- préparation d'équipe et lecture générale des combats de boss.

L'ancien wiki du dépôt `johto-family-remix` / branche `wiki/family-remix-v1` peut être utilisé comme **référence éditoriale** pour récupérer des explications claires, mais toutes les données doivent être revalidées contre `docs/spec/`, `data/spec/` et le code Tactica.

### Changements

La page Changements doit être **exhaustive** et ne contenir que des modifications réellement intégrées/canoniques. Elle couvre :
- systèmes de jeu ;
- 26 Pokémon modifiés ;
- 15 learnsets custom ;
- évolutions ;
- rencontres ;
- boutiques et PNJ ;
- règles de boss / difficulté ;
- autres changements de confort effectivement présents.

Chaque Pokémon modifié est présenté sur une ligne/carte :
`sprite + nom + type(s) + changements précis`.

Les types, stats, talents et learnsets affichés viennent du dataset canonique. Les confirmations de playtest récentes priment sur les anciens textes ; Salamèche et Reptincel Dragon / Feu sont notamment confirmés en jeu.

### Pokédex

Le Pokédex contient les informations de l'espèce et sa recherche localisée. Ne pas remettre de colonne/section « Progression » dans le Pokédex. Les localisations restent sur la page dédiée.

### Localisations

Les Localisations exposent zone, méthode, niveau, taux et jour/nuit si applicable depuis les JSON canoniques. Elles doivent refléter les quatre vrais slots `30/30/30/10` et les résultats du nouvel audit de couverture pré-Ligue/déduplication.

### Boss & Conseils

Boss & Conseils montre le **roster réellement canonique** : sprites, niveaux, objets, talents, attaques et logique stratégique. Les sprites doivent être détourés/avec transparence, sans fond rectangulaire parasite.

Les conseils doivent aider à comprendre le plan du boss sans imposer une séquence de tours. Les équipes affichées suivent les dernières décisions owner, notamment Albert 3, Hector 4, Blanche 6 et la correction Ursaring de Blanche.

### Crédits et Roadmap

Les Crédits sont réservés aux projets amont, contributeurs, licences et reconnaissance du travail de HnS/pokeemerald-expansion.

Le suivi du développement, les étapes V1, la release candidate, le playthrough, les corrections et les éléments repoussés en V2 sont déplacés dans une page **Roadmap** FR/EN. Ne pas mélanger cette roadmap avec les crédits.

## QA visuelle et fonctionnelle

- Corriger les entêtes de tableaux qui recouvrent la première ligne. Si le sticky header n'est pas fiable, utiliser un header non sticky.
- Vérifier les tableaux sur desktop et mobile.
- Vérifier que tous les sprites Boss/Pokémon s'affichent avec transparence correcte.
- Vérifier la recherche avec noms FR, EN et variantes sans accents sur le site FR.
- Vérifier chaque bouton de langue sur chaque page.
- Vérifier qu'aucune page FR ne contient de noms/attaques/objets anglais résiduels.
- Vérifier que le contenu est aligné avec les specs et données courantes, pas seulement que les liens fonctionnent.

Les données du wiki sont générées ou vérifiées contre `data/spec/`, puis contrôlées sur mobile. Le wiki importé ou ancien est une base éditoriale, jamais une source de vérité produit.
