# Pokémon Tactica

Pokémon Tactica reprend la base jouable **Pokémon Heart & Soul (HnS)** pour une aventure Johto/Kanto centrée sur la stratégie : adversaires cohérents, rencontres à quatre slots, outils d'entraînement et accès aux capacités et objets utiles. La V1 est en préparation ; les états détaillés sont dans [STATUS_AT_IMPORT](docs/spec/STATUS_AT_IMPORT.md).

## Contribuer à la V1

- [Source de vérité](docs/spec/SOURCE_OF_TRUTH.md) : choix produit validés et liens vers les données.
- [Prompt de reprise Codex](docs/spec/CODEX_START.md) : audit avant production et travail par blocs.
- [Migration](docs/spec/MIGRATION.md) : provenance, contrôles et branches.
- `data/spec/` : tables de rencontres, boss, starters, rival et Pokémon modifiés.

Build technique : `make hns -j4`. Le nom de la cible `hns` et certains identifiants historiques du moteur sont conservés pour la compatibilité. Une réussite de build ne suffit pas à déclarer la V1 testée ou publiée.

## Base et crédits

Tactica est un fork de [Pokémon Heart & Soul / pokemonHnS-expansion](https://github.com/pokemonhns-development/pokehns-expansion), lui-même bâti avec [Modern Emerald](https://github.com/resetes12/pokeemerald), [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion) et [pokeemerald](https://github.com/pret/pokeemerald). Voir [CREDITS.md](CREDITS.md), les notices et les crédits contenus dans le code. Les documents HnS conservés dans `docs/` sont de la documentation technique amont et ne définissent pas les règles de Tactica.
