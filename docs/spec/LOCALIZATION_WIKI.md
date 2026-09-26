# Interface, traduction et wiki

La direction visuelle est tirée des **valeurs réelles** de la ROM : fond sombre noir/bleu, rouge, bleu et accents clairs. Lire la palette des assets et du code UI avant de fixer les variables CSS, puis vérifier par capture en jeu et dans le navigateur. Ne pas conserver une approximation rouge/noir différente. Préserver l'interface de jeu validée hors correction démontrée.

FR : noms officiels français des Pokémon, attaques, talents, objets, types et personnages ; EN : termes officiels anglais. Relire les textes naturels, sans anglais résiduel en FR (par exemple Famignol, pas Maushold) ni jargon inexpliqué. Le contenu doit être un miroir entre langues. Les sprites Pokémon et Champions accompagnent leurs informations pertinentes.

Navigation : **Guide de jeu**, **Changements**, **Pokédex**, **Localisations**, **Boss & Conseils**, **Crédits/versions** ; Pokédex et Localisations sont deux rubriques distinctes. Le Guide explique concrètement réglages, sélection, captures, entraînement, shops, caps et préparation de combat. Changements inventorie **toutes** les modifications réelles et présente chaque Pokémon modifié sur une ligne/carte `sprite + nom + type(s) + changements précis`. Pokédex offre recherche et fiches FR/EN ; Localisations expose zone, méthode, niveau, taux et éventuel jour/nuit à partir des JSON canoniques ; Boss & Conseils montre le roster réel et des conseils compréhensibles, sans séquence de tours imposée. Les guides d'arène en jeu s'adaptent aux équipes et conservent l'Eau Fraîche selon la logique prévue.

Les données du wiki sont générées ou vérifiées contre `data/spec/`, puis contrôlées sur mobile. Le wiki importé de l'ancienne branche est une **base éditable**, et ses pages construites depuis des données anciennes doivent être régénérées.

`tools/sync_tactica_localization.py` maintient les niveaux des 405 tables standard dans les pages Localisations/Locations Markdown et HTML à partir de `data/spec/encounters_standard.json`. Son mode `--check` est obligatoire en CI après toute modification des rencontres. `tools/validate_tactica_localization.py` contrôle l'identité visible Tactica, le format de l'asset de titre, la présence des pages cœur FR/EN, les liens locaux et le nombre de lignes de rencontres.

Le moteur importé ne définit actuellement que `GAME_LANGUAGE LANGUAGE_ENGLISH`. Le miroir documentaire FR/EN ne tranche pas à lui seul entre deux ROM localisées et un sélecteur de langue dans une ROM unique ; ce choix d'architecture doit être décidé avant d'étendre la traduction aux textes du jeu.
