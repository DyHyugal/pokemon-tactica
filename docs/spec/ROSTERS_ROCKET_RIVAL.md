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

**État de réalisation :** les combats Rocket existants sont générés dans `src/data/trainers_hns.party` par `tools/sync_tactica_bosses.py`, et `tools/validate_family_remix.py` contrôle les équipes des deux difficultés. L'équilibrage en situation reste à jouer dans la ROM, notamment l'accès réel aux capacités par niveau.

## Rival

| Catégorie persistante | Thème | Six familles dans l'ordre des indices |
|---|---|---|
| Feu | Soleil | Goupix, starter conservé, Granivol, Galvaran, Malosse, Caninos |
| Eau | Pluie | Goélise, starter conservé, Nénupiot, Loupio, Cradopaud, Stari |
| Plante | Champ Herbu | Olivini, starter conservé, Trompignon, Passerouge, Cabriolaine, Selutin |
| Électrik | Champ Électrifié | Wattapik, starter conservé, Lixy, Wattouat, Toxizap, Motisma |
| Sol | Sable | Hippopotas, starter conservé, Selutin, Kraknoix, Balbuto, Rapion |
| Glace | Neige | Blizzi, starter conservé, Obalie, Frissonille, Sabelette d'Alola, Piétacé |

Le premier combat conserve uniquement le starter du rival, tiré une fois et sauvegardé. Après le premier badge, les indices 1 à 3 forment l'équipe de trois ; après le deuxième, l'indice 4 s'ajoute ; après le troisième, les indices 5 et 6 complètent l'équipe. Ces seuils décrivent la composition des combats existants selon le jalon atteint : ils ne commandent pas la création d'un nouveau duel. Les espèces effectivement jouées doivent évoluer selon le niveau réel du combat et la disponibilité des objets d'évolution. Les propositions EARLY/MID/FINAL de quatre capacités, de nature, d'EV et d'objet final sont dans `rival.json` ; le starter sauvegardé doit choisir un set légal pour son espèce parmi les trente possibles.

Les combats déjà présents se trouvent dans `CherrygroveCity_hns` (1, niveau 5, un membre), `AzaleaTown_hns` (2, niveau 18, trois), `BurnedTower_1F_hns` (3, niveau 24, quatre), `GoldenrodCity_UndergroundSwitches_hns` (4, niveau 40, six), `VictoryRoadKanto_1F_hns` (5, niveau 48, six), `MtMoon_Cave_hns` (6, niveau 64, six) et `IndigoPlateau_PokemonCenter_hns` (7, niveau 68, six). Chacun possède actuellement trois variantes de starter historique (`CHIKORITA`, `CYNDAQUIL`, `TOTODILE`) dans `trainers_hns.party` ; la catégorie persistante du rival, issue de `VAR_FAMILY_RIVAL_SPECIES`, doit piloter ses **autres** membres sans réécrire le starter sauvegardé. Le combat 4 avait cinq membres dans l'import initial ; la branche `feature/rival-battles-v1` ajoute un sixième membre transitoire de la famille Ursaring, déjà présent au combat 5. Le double objet Scope Lens de la variante Totodile aux combats 6 et 7 est remplacé par Mystic Water sur Aligatueur.

**État de réalisation :** ces six rosters sont rédigés et contrôlés structurellement par `tools/validate_tactica_spec.py`. Leur conversion en parties du moteur, l'adaptation dynamique à la catégorie sauvegardée, les trente starters et la vérification de disponibilité des attaques restent à développer et tester. Ce document est un contrat de travail réutilisable, et ne certifie pas des combats rival déjà implémentés.

## Contrôles avant intégration finale

1. Vérifier les espèces, talents et attaques des sets EARLY/MID au niveau réel de chaque combat ; remplacer tout choix illégal sans perdre l'identité des familles.
2. Confirmer dans le code de scénario l'association des combats aux seuils de badges et la persistance de la catégorie et du starter déjà sauvegardés.
3. Jouer dans mGBA les combats Rocket/rival représentatifs en NORMAL et HARD ; vérifier le cap, la difficulté et le rendu en jeu.
