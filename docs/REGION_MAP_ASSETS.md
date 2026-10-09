# Recherche d’assets — carte régionale Johto/Kanto

Relevé technique au 9 octobre 2026. Ce rapport décrit les sources vérifiées ; il ne définit pas de nouvelle règle de jeu.

Le besoin concerne le dessin des routes, des marqueurs de villes, du relief et de l’eau, avec une esthétique DS/Switch et la géographie existante. Les bandeaux et textes déjà adaptés ne constituent pas cette refonte. Aucun pack plus récent, couvrant Johto et Kanto et prêt à importer dans le moteur GBA, n’a été identifié dans les sources ci-dessous. Aucun asset n’a été importé ni redessiné pendant cette recherche.

| Source vérifiée | Révision consultée | Résultat |
| --- | --- | --- |
| [HnS amont](https://github.com/PokemonHnS-Development/pokehns-expansion/tree/167aa6d537b109bb229c231ddce4616974c4da71/graphics/pokenav/region_map) | `167aa6d537b109bb229c231ddce4616974c4da71` | `map_johto.png` et `map_jk.png` identiques aux images Tactica. |
| [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/7b95be15d84a053948791ce91e5dec0b168947a5/graphics/pokenav/region_map) | `7b95be15d84a053948791ce91e5dec0b168947a5` | Carte Kanto présente, aucun asset Johto ou Johto/Kanto dans ce répertoire. |
| [PKMN-World](https://github.com/evilchinesefood/PKMN-World/tree/15d3888b0d81322a04d3c71b0bce18948761ee44) | `15d3888b0d81322a04d3c71b0bce18948761ee44` | Images Johto et Kanto identiques aux images Tactica, comparaison SHA-256. |
| [Fork TixoRebel](https://github.com/TixoRebel/pokehns-expansion/tree/5c176d922a4c1ac10a0c5fb656b643a6d8b658ab) | `5c176d922a4c1ac10a0c5fb656b643a6d8b658ab` | `johtomap.png` renommée, image Johto identique. |
| [Fork gen4](https://github.com/torbatiidan241-eng/pokehns-expansion-gen4/tree/ead1fada7fdb350ecbb978fafee91f92abf6aeda) | `ead1fada7fdb350ecbb978fafee91f92abf6aeda` | Johto et Johto/Kanto identiques malgré le nom « gen4 ». |
| [pokeemerald-johto-backup](https://github.com/ashytastic/pokeemerald-johto-backup/tree/50f1533632216d1a689e222ed1111cf0896e4ddb) | `50f1533632216d1a689e222ed1111cf0896e4ddb` | Pas de pack Johto/Kanto dans le dossier des cartes régionales. |
| [pokemontrinity](https://github.com/jps7878/pokemontrinity/tree/e7699f9169dff4125d1ce0873323a76b417eb730) | `e7699f9169dff4125d1ce0873323a76b417eb730` | Cartes native et Kanto, aucun pack Johto/Kanto. |

Les recherches GitHub « johto pokeemerald », « pokehns-expansion » et « Johto region map » ont également été consultées. Le résultat [PokemonMapUI](https://github.com/signOfi/PokemonMapUI) est une application web, sans pack GBA compatible vérifié. Cette recherche bornée ne démontre pas qu’aucune ressource n’existe ailleurs.

## Empreintes de comparaison

Les images canoniques sont dans `graphics/pokenav/region_map/` ; leur palette et leurs tilemaps restent liées au rendu du moteur.

| Image Tactica | SHA-256 |
| --- | --- |
| `map_johto.png` | `8cf72765d1132137e0046ec6e4546e3f49abf3f7393c58f64cc50b57fc1a2384` |
| `map_jk.png` | `e368084f9277fd6d11724f5dee8d4f26cd8750fd1bef0622d3d8204161f8e4fd` |
| `map_kanto.png` | `cfcba70948def29cf5d7d0db9c467f2ffe9889f66e3a09ea9a9d662d2008c107` |

Un remplacement ultérieur devra vérifier crédits/licence, palette 4bpp, atlas de tuiles et tilemap, grille des sections et destinations de Vol pour les deux régions. Changer uniquement le PNG ou les couleurs ne garantit pas ces correspondances. En l’absence de remplacement vérifié, le dessin existant est conservé et la modernisation graphique demeure partielle.
