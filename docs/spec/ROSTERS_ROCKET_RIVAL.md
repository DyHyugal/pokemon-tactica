# Équipes progressives : Rocket et rival

**Sources de données :** `data/spec/rocket_progression.json` pour les phases Rocket, `data/spec/bosses.json` pour leurs six membres finaux et `data/spec/rival.json` (`fight_rosters`) pour les six catégories du rival. Les règles générales de niveaux, de familles persistantes, de légalité et d'objets uniques restent dans `STRATEGIC_TEAMS_AND_ACCESS.md`.

## Exécutifs Rocket

| Exécutif | EARLY (3 familles) | MID (4 familles) | FINAL (6 familles) | Combats existants actuellement associés |
|---|---|---|---|---|
| Proton | Nosferapti, Rattata, Tadmorv d'Alola | + Smogo | + Venipatte, Cradopaud | Proton 1 = EARLY ; Proton 2 = FINAL |
| Petrel | Métamorph, Smogo, Rattata | + Voltorbe | + Tadmorv, Zorua de Hisui | Petrel 1 = MID ; Petrel 2 = FINAL |
| Ariana | Abo, Mystherbe, Cornèbre | + Gribouraigne | + Nidoran♀, Tritox | Ariana 1 = MID ; Ariana 2 = FINAL |
| Archer | Farfuret, Nosferapti, Malosse | + Nidoran♂ | + Magnéti, Rapion | Archer = FINAL |

Les stades EARLY et MID sans combat existant sont décrits pour la continuité de conception ; ils ne créent aucun événement de scénario. Les indices `slot` se rapportent à la liste finale et préservent l'identité de chaque famille même si l'ordre tactique change. Les sets, talents, objets et EV EARLY/MID sont définis pour chaque membre dans le JSON ; les six membres finaux et leurs sets restent définis dans `bosses.json`. Le niveau réel des combats Rocket est calculé par le moteur depuis le cap ; `Level: 1` dans la source de parties reste un marqueur technique. NORMAL et HARD partagent espèces, objets et attaques ; HARD ajoute les IV, EV et l'IA loyale.

**État de réalisation :** les combats Rocket existants sont générés dans `src/data/trainers_hns.party` par `tools/sync_tactica_bosses.py`, et `tools/validate_family_remix.py` contrôle les équipes des deux difficultés, l'unicité des objets et la compatibilité de chaque capacité avec l'espèce. L'équilibrage en situation reste à jouer dans la ROM, notamment l'accès réel aux capacités par niveau.

## Rival

| Catégorie persistante | Thème | Six familles dans l'ordre des indices |
|---|---|---|
| Feu | Soleil | Goupix, starter conservé, Granivol, Galvaran, Malosse, Caninos |
| Eau | Pluie | Goélise, starter conservé, Nénupiot, Loupio, Cradopaud, Stari |
| Plante | Champ Herbu | Olivini, starter conservé, Trompignon, Passerouge, Cabriolaine, Selutin |
| Électrik | Champ Électrifié | Wattapik, starter conservé, Lixy, Wattouat, Toxizap, Motisma |
| Sol | Sable | Hippopotas, starter conservé, Selutin, Kraknoix, Balbuto, Rapion |
| Glace | Neige | Blizzi, starter conservé, Obalie, Frissonille, Sabelette d'Alola, Piétacé |

Le premier combat conserve uniquement le starter du rival, tiré une fois et sauvegardé, **mais il se joue au niveau 17** (cap de préparation d'Albert) et le starter prend son stade d'évolution légal à ce niveau. Après le premier badge, les indices 1 à 3 forment l'équipe de trois ; après le deuxième, l'indice 4 s'ajoute ; après le troisième, les indices 5 et 6 complètent l'équipe. Ces seuils décrivent la composition des combats existants selon le jalon atteint : ils ne commandent pas la création d'un nouveau duel. Les espèces effectivement jouées doivent évoluer selon le niveau réel du combat et la disponibilité des objets d'évolution. Les propositions EARLY/MID/FINAL de quatre capacités, de nature, d'EV et d'objet final sont dans `rival.json` ; le starter sauvegardé doit choisir un set légal pour son espèce parmi les trente possibles.

Les combats déjà présents sont les mêmes événements/scripts. Le premier combat de `CherrygroveCity_hns` conserve un membre mais doit être résolu **niveau 17** malgré le template historique niveau 5. Les combats suivants utilisent les jalons canoniques et leurs tailles 3/4/6/6/6/6. Les trois variantes historiques (`CHIKORITA`, `CYNDAQUIL`, `TOTODILE`) restent les points d'entrée des scripts, puis `FamilyStarter_ResolveRivalMon` remplace leur contenu au chargement selon la catégorie persistée dans `VAR_FAMILY_RIVAL_SPECIES`. Le premier combat utilise seulement le starter sauvegardé ; les suivants emploient les phases 3/4/6 du JSON. Le combat 4 avait cinq membres dans l'import initial ; la PR #5 fusionnée à `integration/v1` ajoute le sixième emplacement nécessaire.

**État de réalisation :** `tools/generate_tactica_rival.py` convertit les six rosters en `src/data/tactica_rival.h`. Le runtime dynamique est intégré, mais le premier combat reste à corriger car son template moteur est encore niveau 5. Le moteur résout la catégorie sauvegardée, les phases EARLY/MID/FINAL, les formes permises par le niveau et les conditions de chaque combat, les objets et natures, ainsi que les EV réservés au mode HARD. Le starter choisit ses attaques disponibles au niveau réel et reçoit le booster de sa catégorie dans la phase finale. Les validateurs contrôlent la table générée, l'unicité des sets et objets, la compatibilité de chaque capacité avec la famille concernée et le branchement au créateur d'équipes ; les tests `Family` couvrent les phases et les seuils d'évolution. La recette mGBA NORMAL/HARD reste nécessaire.

## Contrôles avant intégration finale

1. Vérifier les espèces, talents et attaques des sets EARLY/MID au niveau réel de chaque combat ; remplacer tout choix illégal sans perdre l'identité des familles.
2. Confirmer dans le code de scénario l'association des combats aux seuils de badges et la persistance de la catégorie et du starter déjà sauvegardés.
3. Jouer dans mGBA les combats Rocket/rival représentatifs en NORMAL et HARD ; vérifier le cap, la difficulté et le rendu en jeu.
