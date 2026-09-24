# Migration contrôlée

## Provenance gelée

- Code de production connu : ancien dépôt `DyHyugal/johto-family-remix`, `work/native-speed-starter-egg` @ `e5c7191d66a798b3081fd81f95e9155086772c15`.
- Transmission des rencontres : `handoff/v1-clean-source-2026-09-24` @ `4c976ce84a095efe162943c58a21056ac069e59b`, neuf commits au-dessus du code de production ; seules ses données correctes sont migrées, pas ses instructions contradictoires.
- Wiki : `wiki/family-remix-v1` @ `c44f8bde95b811ff01a12173d2392c5cd5a83a81`, branche divergente avec pages et CSS ; import explicite des chemins du wiki, puis correction de palette et de données.

L'ancien dépôt demeure intact comme archive de provenance. Le nouveau dépôt part d'un **instantané propre avec nouvel historique**, sans ancienne documentation normative dans son premier commit. Il n'est pas nécessaire de copier toute l'histoire Git de HnS pour conserver ses crédits/licences : les fichiers de licence/crédits sont maintenus. La documentation de `docs/spec/` et les JSON `data/spec/` sont l'unique contrat Tactica.

## Séquence récupérable

1. Préparer localement l'arbre complet, sans écrire dans l'ancien dépôt distant ; noter les trois SHA ci-dessus.
2. Copier explicitement le wiki ; vérifier qu'aucune modification code de sa branche n'écrase la production.
3. Écarter les anciennes spécifications/handoffs contradictoires, et conserver les composants techniques upstream nécessaires au build. Vérifier absence de fichiers générés privés ou ROM binaire dans l'index.
4. Créer le premier commit local du nouveau dépôt ; noter SHA, nombre de fichiers et manifeste SHA-256 des sources canoniques. Tester que les fichiers sont présents et que l'arbre est propre.
5. Après réception de l'URL du nouveau dépôt vide, pousser **une seule fois** `main` vers cette destination ; vérifier à distance le SHA de la branche. Créer `integration/v1` depuis exactement ce SHA, puis les branches de domaine depuis `integration/v1`.
6. Conserver `main` comme baseline importée contrôlée ; faire toute correction produit via PR de domaine puis assembler et tester dans `integration/v1`. Le nouveau dépôt n'est jamais une cible de `--mirror` ni d'un push de toutes les branches obsolètes.

Branches prévues : `integration/v1`, `feature/encounters`, `feature/rival`, `feature/bosses`, `feature/balance`, `feature/shops`, `feature/localization`, `feature/wiki`. `feature/settings` seulement si l'audit révèle un écart ; ne pas créer des branches pour speed/audio/starter/œuf déjà validés.

## Limites de ce gel

Les rosters complets du rival doivent encore être proposés après inventaire. L'exhaustivité réelle des Move IDs et des items, l'existence d'un marchand de Méga-Gemmes et la palette ROM sont à auditer dans le code. Les données des boss migrées doivent être confrontées aux parties réellement compilées. Ne pas marquer ces points DONE sans preuve.
