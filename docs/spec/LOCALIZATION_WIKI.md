# Interface, traduction et wiki

La direction visuelle custom validée est **noir / rouge / gris**, sans grandes surfaces blanches dominantes. Lire les assets et le code UI avant de fixer les variables CSS, puis vérifier par capture en jeu et dans le navigateur. Préserver l'interface de jeu validée hors correction démontrée.

FR : noms officiels français des Pokémon, attaques, talents, objets, types et personnages ; EN : termes officiels anglais. Relire les textes naturels, sans anglais résiduel en FR (par exemple Famignol, pas Maushold) ni jargon inexpliqué. Le contenu doit être un miroir entre langues. Les sprites Pokémon et Champions accompagnent leurs informations pertinentes.

Navigation : **Guide de jeu**, **Changements**, **Pokédex**, **Localisations**, **Boss & Conseils**, **Crédits**, **Roadmap** ; Pokédex et Localisations sont deux rubriques distinctes. Le Guide explique concrètement réglages, sélection, captures, entraînement, shops, caps et préparation de combat. Changements inventorie **toutes** les modifications réelles et présente chaque Pokémon modifié sur une ligne/carte `sprite + nom + type(s) + changements précis`. Pokédex offre recherche et fiches FR/EN ; Localisations expose zone, méthode, niveau, taux et éventuel jour/nuit à partir des JSON canoniques ; Boss & Conseils montre le roster réel et des conseils compréhensibles, sans séquence de tours imposée. Les guides d'arène en jeu s'adaptent aux équipes et conservent l'Eau Fraîche selon la logique prévue.

Les données du wiki sont générées ou vérifiées contre `data/spec/`, puis contrôlées sur mobile. Le wiki importé de l'ancienne branche est une **base éditable**, et ses pages construites depuis des données anciennes doivent être régénérées.

`tools/sync_tactica_localization.py` maintient les niveaux des 405 tables standard dans les pages Localisations/Locations Markdown et HTML à partir de `data/spec/encounters_standard.json`. Son mode `--check` est obligatoire en CI après toute modification des rencontres. `tools/validate_tactica_localization.py` contrôle l'identité visible Tactica, le format de l'asset de titre, la présence des pages cœur FR/EN, les liens locaux et le nombre de lignes de rencontres.

Le moteur importé ne définit actuellement que `GAME_LANGUAGE LANGUAGE_ENGLISH`. Le miroir documentaire FR/EN ne tranche pas à lui seul entre deux ROM localisées et un sélecteur de langue dans une ROM unique ; ce choix d'architecture doit être décidé avant d'étendre la traduction aux textes du jeu.

## Pokédex joueur — fiche détaillée centrée ROM

La page Pokédex ne doit pas être un simple index de noms. Elle sert de référence de construction d'équipe **strictement pour Pokémon Tactica**.

Pour chaque espèce, la fiche détaillée doit exposer uniquement les informations réellement pertinentes dans la ROM :
- lignée évolutive complète ;
- conditions d'évolution réellement utilisées par Tactica ;
- statistiques de base HP / Atk / Def / SpA / SpD / Spe et total ;
- type(s) ;
- talent(s) disponibles dans la ROM ;
- attaques apprises par niveau dans la ROM, avec niveau et nom localisé ;
- localisation(s) réellement disponibles dans Tactica, avec méthode, niveaux, taux et jour/nuit quand pertinent ;
- éventuelles modifications Tactica de type, stats, talent ou learnset clairement signalées.

Ne pas importer de contenu encyclopédique externe sans rapport avec la ROM : anime, cartes, autres jeux Pokémon, lore historique ou anciennes localisations n'ont pas leur place ici. Poképédia, Insurgence, Decay et autres wikis servent uniquement de **référence ergonomique / éditoriale**, jamais de source de données produit.

### Interaction attendue

Conserver l'index actuel avec recherche et cartes compactes.

- Le lien **Voir les localisations / See locations** reste une action dédiée et mène uniquement à la page Localisations filtrée sur l'espèce.
- Cliquer sur le **nom, le sprite ou le reste de la carte Pokémon** ouvre la fiche détaillée de cette espèce.
- UX privilégiée : **panneau/modal détaillé dans la même page**, avec URL partageable via query/hash et historique navigateur, afin de conserver la recherche et la position dans le Pokédex au retour.
- Sur mobile, le panneau devient une vue plein écran lisible.
- Éviter d'ouvrir automatiquement un nouvel onglet : le joueur garde le contrôle de sa navigation.
- Si une fiche dédiée statique s'avère plus robuste pour la génération ou l'accessibilité, elle peut être utilisée à condition que le retour au Pokédex conserve le filtre/recherche autant que possible.

### Organisation recommandée de la fiche

1. En-tête : sprite, nom FR/EN selon langue, type(s), famille/lignée.
2. Statistiques : six stats + total, avec une visualisation compacte et lisible.
3. Talents : liste des talents réellement disponibles dans la ROM.
4. Évolution : chaîne visuelle + condition exacte entre chaque stade.
5. Attaques par niveau : tableau niveau / attaque / type / catégorie / puissance / précision si ces données sont déjà disponibles dans le moteur.
6. Localisations : résumé des premières zones pertinentes + bouton vers la page Localisations filtrée ; si l'espèce n'est obtenue que par évolution, l'indiquer clairement.
7. Bloc **Modifié dans Tactica** uniquement si l'espèce fait partie des changements canoniques.

Toutes ces données doivent être dérivées du dépôt Tactica : `species_info`, évolutions compilées, learnsets compilés, talents, `data/spec/` et tables de rencontres synchronisées. Aucun copier-coller manuel de données externes.


## Corrections éditoriales owner — 26 septembre 2026

- Le **second starter** n'est pas déclenché par l'assistant d'Orme à Mauville. Il est choisi **au laboratoire d'Orme**, au retour de chez M. Pokémon, après la séquence du vol / police, lorsque le joueur reconnaît l'espèce de l'Œuf Mystère.
- Les starters doivent être présentés **par groupes de type** : Feu, Eau, Plante, Électrik, Sol, Glace, puis Évoli séparément. Éviter une grille plate de 31 cartes sans séparation visuelle.
- Le remplacement du trio Johto par le sélecteur **30 starters + Évoli** et le second choix via l'œuf sont des changements majeurs et doivent apparaître explicitement dans **Changements**.
- La page **Changements** doit décrire les systèmes de façon concrète et orientée joueur. Exemple Boutiques : présenter séparément les **trois PNJ ajoutés** — PNJ Objets, PNJ Capacités/CT, PNJ Méga-Gemmes — avec rôle, limites et lien avec la progression.
- La version FR ne doit pas exposer les noms de zones anglais dans **Localisations**. Les noms de villes, grottes, tours, forêts, îles et zones Safari doivent être localisés en français, tout en conservant les alias anglais dans la recherche quand utile.
- Les fiches Pokédex FR ne doivent jamais afficher un nom ou une description de talent en anglais. Si une description française fiable n'est pas disponible, afficher le **nom français du talent sans la description anglaise** plutôt que mélanger les langues.
- Les références externes de type Poképédia / Insurgence / Decay servent uniquement à l'ergonomie et à l'organisation. Les données de la fiche restent exclusivement celles de la ROM Tactica.


## Routes et Villes / Routes & Cities

La navigation joueur comprend une rubrique dédiée **Routes et Villes** distincte de `Localisations`. `Localisations` répond à « où capturer ce Pokémon ? » ; `Routes et Villes` répond à « que faire et que récupérer dans cette zone ? ».

Chaque fiche de route/ville peut présenter, uniquement lorsque l’information est vérifiée dans la ROM Tactica : connexions et rôle dans la progression, étapes d’histoire, points d’intérêt et services, objets/récompenses, échanges ou Pokémon cadeaux, prérequis d’exploration et rencontres sauvages. Les rencontres restent dérivées des tables canoniques existantes et ne sont pas recopiées manuellement.

Ergonomie inspirée des wikis Pokémon : index filtrable + fiche synthétique, puis sections histoire, points d’intérêt, objets, échanges/cadeaux et rencontres. Les wikis externes ne sont jamais une source de vérité sur le contenu Tactica.
