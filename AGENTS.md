# Instructions pour Codex dans Pokémon Tactica

Lire `docs/spec/SOURCE_OF_TRUTH.md` puis les documents de `docs/spec/` et les JSON de `data/spec/` avant tout changement produit. Ce sont les seules sources normatives Tactica. Les autres fichiers décrivent l'implémentation ou l'amont HnS. En cas de conflit, rapporter l'écart et suivre la spec ; en cas de manque, demander la décision produit sans l'inventer.

Commencer une session de production par l'audit prescrit dans `docs/spec/CODEX_START.md`. Travailler par branche fonctionnelle issue d'`integration/v1`, avec tests ciblés et PR. Préserver les fonctionnalités déjà validées sauf défaut prouvé. Maintenir licences et crédits de l'amont. Les anciens noms visibles au joueur doivent être remplacés par Pokémon Tactica ; les identifiants techniques HnS nécessaires peuvent rester.

Lire `docs/spec/MIGRATION.md` avant de diagnostiquer une régression : le nouveau dépôt a un historique distinct de l'ancienne production. Maintenir `docs/spec/FEATURE_STATUS.md` avec **un tag par feature**, des preuves et les résultats des tests post-migration ; une validation faite uniquement dans l'ancien dépôt reste `[À RECONTRÔLER]` ici.
