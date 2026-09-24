# État initial à vérifier par Codex

| Domaine | Preuve d'import | État attendu à l'audit |
|---|---|---|
| Audio, vitesse x1–x4, Shiny Rate, starter/œuf, évolutions solo | Production @ `e5c7191` et tests antérieurs | Implémentés et protégés ; confirmer non-régression |
| Encounters standard | 405 tables sources quatre slots ; code encore mappé en 12/5/10 dans `src/data/wild_encounters.json` et validateur hérité | À corriger dans code, données compilées et tests |
| Safari / Headbutt | 53 + 4 pools directs dans la production et source vérifiée | Implémenté ; contrôler rotation et interface |
| Rocket | `src/caps.c` fait progresser `progressionReference` après un Rocket | À corriger selon dernier cap champion/rival +2 |
| Rival | `src/family_starter.c` contient `sCounterCategory` avec un seul contre-type par catégorie | À corriger ; rosters thématiques à définir après inventaire |
| Boss NORMAL/HARD et IA | Partie production antérieure ; `data/spec/bosses.json` migré du dataset v8 et corrigé | Comparer ligne par ligne et tester ; Jasmine/Méga et soins à corriger |
| Balance custom | Dataset v4 migré, Dracaufeu +10 supplémentaire | Comparer code espèces et 15 learnsets ; appliquer écart |
| Shops | PNJ CT et objets existent dans les scripts Goldenrod, catalogues incomplets présumés ; PNJ gemmes non établi | Inventaire exhaustif, puis implémentation |
| Wiki | Branche importée ; structure et CSS demandent corrections | Localisations/Pokédex séparés, palette ROM, exhaustivité, FR/EN/mobile |

Ce tableau est une **photographie de l'import**, pas le résultat final de l'audit. Le suivi vivant avec un tag par feature est dans [FEATURE_STATUS.md](FEATURE_STATUS.md) ; Codex actualise ce registre avec chemins, commits et preuves actuelles avant d'écrire le premier bloc de gameplay. La procédure de diagnostic des fonctions auparavant validées est dans [MIGRATION.md](MIGRATION.md).
