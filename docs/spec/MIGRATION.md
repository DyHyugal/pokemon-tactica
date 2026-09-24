# Migration contrôlée

## Provenance gelée

- Code de production connu : ancien dépôt `DyHyugal/johto-family-remix`, `work/native-speed-starter-egg` @ `e5c7191d66a798b3081fd81f95e9155086772c15`.
- Transmission des rencontres : `handoff/v1-clean-source-2026-09-24` @ `4c976ce84a095efe162943c58a21056ac069e59b`, neuf commits au-dessus du code de production ; seules ses données correctes sont migrées, pas ses instructions contradictoires.
- Wiki : `wiki/family-remix-v1` @ `c44f8bde95b811ff01a12173d2392c5cd5a83a81`, branche divergente avec pages et CSS ; import explicite des chemins du wiki, puis correction de palette et de données.

L'ancien dépôt demeure intact comme archive de provenance. Le nouveau dépôt part d'un **instantané propre avec nouvel historique**, sans ancienne documentation normative dans son premier commit. Il n'est pas nécessaire de copier toute l'histoire Git de HnS pour conserver ses crédits/licences : les fichiers de licence/crédits sont maintenus. La documentation de `docs/spec/` et les JSON `data/spec/` sont l'unique contrat Tactica.

## Résultat de la migration et diagnostic d'une régression

Le dépôt de travail est désormais `https://github.com/DyHyugal/pokemon-tactica` : `main` et `integration/v1` ont été créées depuis le même arbre importé (`f5a2a3d14adf1bc3b2394c3c8c29b34d08bcb33a`, arbre `802b11e3a8dad2b2d44a265a36e2272030246952`). Les branches `feature/encounters`, `feature/rival`, `feature/bosses`, `feature/balance`, `feature/shops` et `feature/localization` démarrent sur cet arbre. `feature/wiki` porte un travail documentaire additionnel (`a452998bfabd1643f620bea9ba80547c428b9fa2` au gel) **non fusionné** dans `integration/v1`. Ces SHA sont des points de provenance, et non des marqueurs permanents de la tête des branches après les prochains commits.

La migration a conservé les sources techniques et les crédits, mais a remplacé treize anciens documents produit contradictoires par la spec Tactica. **L'historique Git de l'ancien dépôt n'a pas été recopié** : des SHA de commits différents entre ancien et nouveau dépôts ne prouvent ni régression ni modification du code. Le changement de dépôt, de branche de référence, de documentation, de configuration ou d'environnement de build peut faire apparaître un défaut sur une fonction antérieurement validée. Un test ancien ne vaut pas validation dans le nouveau dépôt.

Si l'audio, la vitesse native, le taux shiny, le starter/œuf ou les évolutions solo échouent désormais :

1. Relever le SHA Tactica, la branche, l'état Git, le build et les étapes de reproduction ; confirmer le même scénario de test et distinguer code, données, configuration, assets et environnement.
2. Comparer **les fichiers de la fonction concernée** avec l'ancienne production `work/native-speed-starter-egg` @ `e5c7191d66a798b3081fd81f95e9155086772c15`, depuis un checkout/archive distinct de `DyHyugal/johto-family-remix`. Le handoff `4c976ce` inclut d'autres travaux : ne pas le prendre pour la seule référence de production de ces fonctions.
3. Rejouer le test ciblé dans Tactica et, si possible, dans l'ancien checkout. Vérifier la compilation et les conditions de test avant d'attribuer une cause à la migration. Un diff identique peut quand même exposer un problème de dépendance ou d'environnement.
4. Documenter les preuves dans `FEATURE_STATUS.md`, taguer la feature `[À CORRIGER]` si le défaut est confirmé et corriger le périmètre minimal sur une branche dédiée. Préserver les autres fonctions validées ; ne pas refondre le moteur audio/vitesse faute de preuve.

Une feature redevient `[DONE]` après correction intégrée et contrôles post-migration documentés. La spec produit prévaut pour tout écart fonctionnel voulu par Tactica.

## Séquence de migration (historique)

1. Préparer localement l'arbre complet, sans écrire dans l'ancien dépôt distant ; noter les trois SHA ci-dessus.
2. Copier explicitement le wiki ; vérifier qu'aucune modification code de sa branche n'écrase la production.
3. Écarter les anciennes spécifications/handoffs contradictoires, et conserver les composants techniques upstream nécessaires au build. Vérifier absence de fichiers générés privés ou ROM binaire dans l'index.
4. Créer le premier commit local du nouveau dépôt ; noter SHA, nombre de fichiers et manifeste SHA-256 des sources canoniques. Tester que les fichiers sont présents et que l'arbre est propre.
5. Publier l'arbre complet sur `main` ; vérifier à distance le SHA de son arbre. Créer `integration/v1` depuis exactement cet arbre, puis les branches de domaine depuis `integration/v1`.
6. Conserver `main` comme baseline importée contrôlée ; faire toute correction produit via PR de domaine puis assembler et tester dans `integration/v1`. Le nouveau dépôt n'est jamais une cible de `--mirror` ni d'un push de toutes les branches obsolètes.

Branches prévues : `integration/v1`, `feature/encounters`, `feature/rival`, `feature/bosses`, `feature/balance`, `feature/shops`, `feature/localization`, `feature/wiki`. `feature/settings` seulement si l'audit révèle un écart ; ne pas créer des branches pour speed/audio/starter/œuf déjà validés.

## Limites de ce gel

Les rosters complets du rival doivent encore être proposés après inventaire. L'exhaustivité réelle des Move IDs et des items, l'existence d'un marchand de Méga-Gemmes et la palette ROM sont à auditer dans le code. Les données des boss migrées doivent être confrontées aux parties réellement compilées. Ne pas marquer ces points DONE sans preuve.
