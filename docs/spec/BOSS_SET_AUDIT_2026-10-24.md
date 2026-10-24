# Audit des sets de boss — 24 octobre 2026

## Résultat et périmètre

Point de départ : `integration/v1`, commit `d41903bad93c07946b480904298856895bff7cd1`.

L'audit couvre **117 identifiants de boss, 167 variantes et 843 emplacements rédigés**, dans toutes les classes protégées par `trainer_levels.json` : Champions Johto/Kanto, rematches, Conseil 4, Maîtres, Rocket, rival historique, Red, Eusine, Giovanni, Kimono Girls, cerveaux de la Zone de Combat et rencontres spéciales. Les ensembles non utilisés sont conservés et contrôlés aussi.

En complément : **les six archétypes dynamiques du rival**, leurs 36 sets finaux en NORMAL/HARD (72 emplacements), leurs formes intermédiaires aux sept combats, et les sept équipes Rocket aux neuf états de progression Johto.

Avant correction : **44 écarts** dans les données rédigées (2 talents, 41 occurrences de capacités incompatibles et 1 répartition EV). Certaines corrections du rival historique sont dupliquées automatiquement en HARD. Après correction : **zéro écart détecté dans ce périmètre**. Le cas distinct de Puissance Cachée Glace est corrigé aussi.

## Major Bob — cause du plantage et correction

Son Élecsprint porte une Ceinture Pro, pas une Élecsprintite. Intimidation est un talent de **Méga-Élecsprint**, pas d'Élecsprint : la construction de l'équipe déclenchait donc l'assertion `illegal ability INTIMIDATE for MANECTRIC` signalée par la capture owner.

Le set utilise maintenant **Paratonnerre**, légal sur sa forme de base, en NORMAL comme en HARD. Méga-Raichu Y reste la Méga de cette équipe. L'assertion moteur est conservée ; aucun remplacement silencieux de talents ne masque les erreurs de données.

Autre défaut du même set : le générateur supprimait « Ice » de `Hidden Power Ice` sans régler les IV. Tous les IV pairs à 24 en NORMAL donnent Combat ; tous les IV à 31 en HARD donnent Ténèbres, jamais Glace. Le contrat HARD impose IV 31. **Rayon Signal** remplace cette couverture annoncée mais irréalisable, sans modifier les IV ni les learnsets. Le générateur refuse désormais une Puissance Cachée typée plutôt que d'en supprimer silencieusement le type.

Sources modifiées : `data/spec/bosses.json`, génération NORMAL/HARD dans `trainers_hns.party`, pages Boss FR/EN et miroirs wiki.

## Corrections des équipes héritées

Les autres corrections portent sur la source rédigée `src/data/trainers_hns.party`, sans changement d'espèce, de niveau, d'objet ou de nature. Les copies HARD du rival sont régénérées par le synchroniseur existant. Le détail machine est enregistré dans `data/spec/boss_set_legality.json`.

Les remplacements conservent autant que possible le rôle de la capacité : priorité pour Roucarnage, setup pour Galeking, soin pour Noadkoko/Ursaking/Noctali, couverture Électrik pour Alakazam, attaque physique après Malédiction pour Flagadoss. Mr. Rime utilise Grêle, présente dans ses données, plutôt qu'une capacité Neige introduite après sa dernière génération officielle. Cette correction ne modifie pas les règles globales de météo du moteur.

| Combat source | Pokémon | Avant | Après |
|---|---|---|---|
| `TRAINER_FALKNER_2_HNS` | Pidgeot | Extreme Speed | Quick Attack |
| `TRAINER_JASMINE_2_HNS` | Aggron | Swords Dance | Hone Claws |
| `TRAINER_CLAIR_2_HNS` | Shelgon | Thunder Wave | Protect |
| `TRAINER_BLUE_2_HNS` | Alakazam | Thunderbolt | Shock Wave |
| `TRAINER_BLUE_2_HNS` | Gyarados | Reversal | Flail |
| `TRAINER_WILL_2_HNS` | Exeggutor | Soft Boiled | Synthesis |
| `TRAINER_WILL_2_HNS` | Slowbro | Fire Punch | Drain Punch |
| `TRAINER_RIVAL_CHIKORITA_6_HNS` | Ursaluna Bloodmoon | Ancient Power | Earth Power |
| `TRAINER_RIVAL_CHIKORITA_7_HNS` | Ursaluna Bloodmoon | Ancient Power | Earth Power |
| `TRAINER_RIVAL_CYNDAQUIL_4_HNS` | Victreebel | Sludge | Acid |
| `TRAINER_RIVAL_CYNDAQUIL_6_HNS` | Ursaluna Bloodmoon | Ancient Power | Earth Power |
| `TRAINER_RIVAL_CYNDAQUIL_7_HNS` | Ursaluna Bloodmoon | Ancient Power | Earth Power |
| `TRAINER_RIVAL_TOTODILE_3_HNS` | Victreebel | Sludge | Acid |
| `TRAINER_RIVAL_TOTODILE_4_HNS` | Victreebel | Sludge | Acid |
| `TRAINER_RIVAL_TOTODILE_6_HNS` | Ursaluna Bloodmoon | Ancient Power | Earth Power |
| `TRAINER_RIVAL_TOTODILE_7_HNS` | Ursaluna Bloodmoon | Ancient Power | Earth Power |
| `TRAINER_EUSINE_HNS` | Electrode | Body Slam | Swift |
| `TRAINER_GIOVANNI_HNS` | Ursaluna | Sandstorm | Bulk Up |
| `TRAINER_ARCHER_4_HNS` | Porygon Z | Flamethrower | Shadow Ball |
| `TRAINER_ARCHER_4_HNS` | Gyarados | Gust | Twister |
| `TRAINER_ARCHER_5_HNS` | Porygon Z | Flamethrower | Shadow Ball |
| `TRAINER_ARCHER_5_HNS` | Gyarados | Gust | Twister |
| `TRAINER_PHANTONOMY_UNUSED_HNS` | Noivern | 255 SpA / 6 SpD / 255 Spe | 252 SpA / 6 SpD / 252 Spe |
| `TRAINER_FALKNER_POSTOBC_HNS` | Pidgeot | Extreme Speed | Quick Attack |
| `TRAINER_WHITNEY_POSTOBC_HNS` | Ursaluna-Bloodmoon | Lunar Blessing | Moonlight |
| `TRAINER_PRYCE_POSTOBC_HNS` | Mr. Rime | Snowscape | Hail |
| `TRAINER_CLAIR_POSTOBC_HNS` | Dudunsparce | Tailwind | Agility |
| `TRAINER_BROCK_POSTOBC_HNS` | Relicanth | Wave Crash | Waterfall |
| `TRAINER_BROCK_POSTOBC_HNS` | Relicanth | Thunderbolt | Earthquake |
| `TRAINER_WILL_POSTOBC_HNS` | Slowbro | Fire Punch | Drain Punch |
| `TRAINER_KOGA_POSTOBC_HNS` | Nidoking | Noble Roar | Sludge Wave |
| `TRAINER_BRUNO_POSTOBC_HNS` | Hitmontop | Victory Dance | Bulk Up |
| `TRAINER_KAREN_POSTOBC_HNS` | Umbreon | Lunar Blessing | Moonlight |

## Méga-Évolutions : vérification des transformations

Les **46 emplacements Méga rédigés** et les **12 emplacements Méga finaux dynamiques du rival** représentent **28 couples distincts de forme / pierre / talent de base**. Tous passent une transformation réelle dans le moteur de combat, côté adversaire et côté joueur :

- la forme envoyée et son talent d'entrée sont légaux ;
- la pierre mène à une seule forme Méga active ;
- le talent après transformation correspond au slot réellement conservé, y compris un talent caché ;
- les deux types sont mis à jour, y compris la perte d'un second type ;
- les formes du joueur et leurs talents reviennent correctement après le combat ;
- aucune équipe contrôlée ne porte plusieurs Méga-Gemmes.

Les données custom validées priment sur un format officiel strict : **Méga-Dracaufeu Y reste Feu/Dragon**, comme prévu dans `pokemon_balance.json`. Méga-Jungko conserve Chlorophylle. Ces identités sont protégées, pas « corrigées » vers les formes officielles. Les Méga récentes disponibles dans cette base sont testées selon leurs données actives.

| Forme de base | Talent avant → après | Types avant → après | Pierre |
|---|---|---|---|
| ABOMASNOW | SNOW_WARNING → SNOW_WARNING | GRASS / ICE → GRASS / ICE | ABOMASITE |
| ABSOL | SUPER_LUCK → MAGIC_BOUNCE | DARK → DARK | ABSOLITE |
| AGGRON | STURDY → FILTER | STEEL / ROCK → STEEL | AGGRONITE |
| ALAKAZAM | MAGIC_GUARD → TRACE | PSYCHIC → PSYCHIC | ALAKAZITE |
| ALTARIA | NATURAL_CURE → PIXILATE | DRAGON / FLYING → DRAGON / FAIRY | ALTARIANITE |
| BANETTE | CURSED_BODY → PRANKSTER | GHOST → GHOST | BANETTITE |
| BAXCALIBUR | THERMAL_EXCHANGE → THERMAL_EXCHANGE | DRAGON / ICE → DRAGON / ICE | BAXCALIBRITE |
| BEEDRILL | SWARM → ADAPTABILITY | BUG / POISON → BUG / POISON | BEEDRILLITE |
| BLAZIKEN | BLAZE → SPEED_BOOST | FIRE / FIGHTING → FIRE / FIGHTING | BLAZIKENITE |
| CHARIZARD | BLAZE → TOUGH_CLAWS | FIRE / DRAGON → FIRE / DRAGON | CHARIZARDITE_X |
| CHARIZARD | SOLAR_POWER → DROUGHT | FIRE / DRAGON → FIRE / DRAGON | CHARIZARDITE_Y |
| DRAGALGE | POISON_TOUCH → ADAPTABILITY | POISON / DRAGON → POISON / DRAGON | DRAGALGITE |
| GARDEVOIR | TRACE → PIXILATE | PSYCHIC / FAIRY → PSYCHIC / FAIRY | GARDEVOIRITE |
| GENGAR | CURSED_BODY → SHADOW_TAG | GHOST / POISON → GHOST / POISON | GENGARITE |
| HERACROSS | GUTS → SKILL_LINK | BUG / FIGHTING → BUG / FIGHTING | HERACRONITE |
| HOUNDOOM | FLASH_FIRE → SOLAR_POWER | DARK / FIRE → DARK / FIRE | HOUNDOOMINITE |
| MANECTRIC | LIGHTNING_ROD → INTIMIDATE | ELECTRIC → ELECTRIC | MANECTITE |
| MEDICHAM | PURE_POWER → PURE_POWER | FIGHTING / PSYCHIC → FIGHTING / PSYCHIC | MEDICHAMITE |
| PIDGEOT | BIG_PECKS → NO_GUARD | NORMAL / FLYING → NORMAL / FLYING | PIDGEOTITE |
| RAICHU | LIGHTNING_ROD → LIGHTNING_ROD | ELECTRIC → ELECTRIC | RAICHUNITE_Y |
| SCEPTILE | CHLOROPHYLL → CHLOROPHYLL | GRASS / DRAGON → GRASS / DRAGON | SCEPTILITE |
| SCOLIPEDE | SPEED_BOOST → SPEED_BOOST | BUG / POISON → BUG / POISON | SCOLIPITE |
| SHARPEDO | SPEED_BOOST → STRONG_JAW | WATER / DARK → WATER / DARK | SHARPEDONITE |
| STARMIE | ANALYTIC → ANALYTIC | WATER / PSYCHIC → WATER / PSYCHIC | STARMINITE |
| STEELIX | STURDY → SAND_FORCE | STEEL / GROUND → STEEL / GROUND | STEELIXITE |
| SWAMPERT | TORRENT → SWIFT_SWIM | WATER / GROUND → WATER / GROUND | SWAMPERTITE |
| TYRANITAR | SAND_STREAM → SAND_STREAM | ROCK / DARK → ROCK / DARK | TYRANITARITE |
| VENUSAUR | CHLOROPHYLL → THICK_FAT | GRASS / POISON → GRASS / POISON | VENUSAURITE |

Aucun nouveau défaut de transition talent/type n'a été observé sur ces 28 couples. Le correctif Ectoplasma Corps Maudit → Méga-Ectoplasma Marque Ombre est couvert explicitement par ces tests et par le test IA existant.

## Méthode et limites de la légalité

`tools/validate_tactica_boss_sets.py` utilise la configuration HnS active après prétraitement C, les capacités compatibles historiques `all_learnables.json`, les listes natives niveau/CT/tuteur/œuf, les pré-évolutions et les attaques propres aux formes de Motisma. Il résout les anciens alias de noms. Il vérifie aussi espèces actives, talents de base, noms d'objets/capacités, doublons, maximum quatre capacités, bornes IV/EV, pierres et talents Méga canoniques.

Le contrôle des capacités est **une compatibilité Tactica**, avec ses ajouts approuvés et ses apprentissages historiques. Ce n'est pas un certificat de légalité d'un format compétitif officiel d'une génération unique : disponibilité des événements, transfert entre cartouches et combinaisons d'événements ne sont pas simulés. Les capacités de niveau futures déjà autorisées dans les sets fixes ne sont pas retirées arbitrairement.

Trois capacités historiques étaient absentes du JSON général mais ont une source vérifiable : Croissance sur Évoli événement 2G (héritée par Noctali), Grobisou sur Nidoran mâle événement 2G (hérité par Nidoking), Berceuse sur Évoli événement 5G (héritée par Nymphali). Elles sont conservées avec leur provenance, sans élargir les learnsets du joueur. Les événements 2G ne sont pas prétendus transférables dans les jeux officiels récents.

Références examinées le 24 octobre 2026 :

- [Pokémon Showdown — learnsets historiques 2G](https://github.com/smogon/pokemon-showdown/blob/master/data/mods/gen2/learnsets.ts) : Évoli `growth: 2S0`, Nidoran mâle `lovelykiss: 2S0`.
- [Pokémon Showdown — learnsets](https://github.com/smogon/pokemon-showdown/blob/master/data/learnsets.ts) : Évoli `sing: 5S2` et données d'événement associées.
- Sources normatives Tactica : `BOSSES_AI.md`, `POKEMON_BALANCE.md`, `bosses.json`, `rival.json`, `rocket_progression.json`.

Le helper runtime `IsTacticaMoveLegalAtLevel` sert à adapter une forme intermédiaire à son niveau. Il ne représente pas à lui seul tous les anciens tuteurs et héritages autorisés pour un set fixe ; il est donc réservé aux tests des équipes dynamiques effectivement adaptées par le moteur.

## Validation reproductible

```sh
python3 tools/validate_tactica_boss_sets.py --report /tmp/tactica-boss-audit.json
python3 -m unittest discover -s tools/tests -p 'test_tactica_*.py'
python3 tools/validate_tactica_spec.py
python3 tools/validate_family_remix.py
python3 tools/validate_tactica_balance.py
python3 tools/validate_tactica_mega.py
python3 tools/sync_tactica_bosses.py --check
python3 tools/sync_tactica_rival_parties.py --check
python3 tools/sync_tactica_boss_wiki.py --check
make hns -j4
make check BUILD=hns TESTS='Tactica boss legality:' -j4
make check BUILD=hns TESTS=Family -j4
make check BUILD=hns TESTS='*Level cap:' -j4
make check BUILD=hns TESTS='Morty-style Gengar Mega Evolution*' -j4
```

Le nouveau contrôle statique et les tests moteur sont ajoutés aux gates CI de PR et d'intégration. Les 28 cas Méga sont figés dans `test/battle/tactica_boss_mega_cases.h`, régénérables explicitement via `--write-mega-fixture` ; une dérive non régénérée fait échouer la validation.

Contrôle manuel restant : jouer Major Bob avec une sauvegarde en jeu dans chaque difficulté, puis observer notamment Mortimer, Jasmine, Sandra et les Méga du rival. Les assertions automatisées ne remplacent pas ce parcours owner. Aucun format de sauvegarde n'est changé ; charger une sauvegarde du menu du jeu plutôt qu'un ancien état instantané d'émulateur.
