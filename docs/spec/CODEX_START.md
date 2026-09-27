# Reprise Codex — Pokémon Tactica V1

## Démarrage obligatoire

Toujours repartir du dernier `integration/v1` :

```bash
git fetch origin
git switch integration/v1
git pull --ff-only origin integration/v1
git rev-parse HEAD
```

Puis lire, dans cet ordre :

1. `AGENTS.md`
2. `docs/spec/SOURCE_OF_TRUTH.md`
3. `docs/spec/FEATURE_STATUS.md`
4. `docs/spec/PLAYTEST_STATUS.md`
5. `docs/spec/EVOLUTIONS.md` si le sujet touche une évolution, un objet d'évolution, une forme ou le Pokédex
6. les autres documents métier concernés
7. les `data/spec/*.json` concernés

Une branche ou PR non fusionnée n'est jamais l'état testable owner.

## Règle documentaire

Les documents actifs doivent décrire uniquement l'état courant. Ne pas conserver plusieurs couches « décision initiale / correction / correction finale » dans un même fichier : l'historique appartient à Git.

Hiérarchie : `SOURCE_OF_TRUTH.md` → docs métier → JSON canoniques → runtime/généré.

## État courant à protéger

Ne pas recoder ces blocs sans défaut démontré sur le HEAD courant :

- vitesse native x1/x2/x3/x4 ;
- audio indépendant ;
- Shiny Rate ;
- sélection des 30 starters + Évoli ;
- curseur initial du sélecteur et retour après annulation ;
- œuf d'Orme distinct du starter principal ;
- encounters à quatre slots `30/30/30/10` ;
- règle du premier accès réel ;
- Scorplane Route 34 jour, niveaux 25–28, après badge 2 ;
- Wattouat Route 31 jour ;
- couverture standard pré-Ligue complète ;
- boss, Conseil 4 et Rocket issus des sources canoniques ;
- exactement une Méga par Champion à partir de Mortimer ;
- unicité globale des Méga importantes ;
- Jeannine : Aéromite reste l'ace, Méga-Kravarech remplace Gaulet ;
- progression Kanto 75 / 80 / 85 / 90 / 95 / 100 ;
- deuxième Ligue et match retour du Maître au niveau 100 ;
- baseline wiki fusionnée depuis la PR #29, y compris Routes/Villes ;
- évolutions canoniques et Fil Liaison, protégées par les validateurs et le synchroniseur Pokédex.

## Blocs désormais terminés côté développement

### UI/UX

Les corrections de runtime/assets déjà intégrées ne doivent pas être réécrites avant la prochaine ROM candidate :

- menu principal Start ;
- Summary ;
- HUD combat ;
- boutiques ;
- header Options / `B SAVE & EXIT`.

Ces écrans restent à **observer dans une ROM fraîche**. Une validation statique verte ne vaut pas validation visuelle.

### Difficulty

Le bloc Difficulty est implémenté côté runtime/menu et doit maintenant être **testé en ROM**, notamment visibilité, sélection et persistance.

Ne pas modifier Vitesse, Audio ou Shiny Rate sans défaut démontré.

### Méga-Évolution

Le bloc Méga a été corrigé côté runtime et couvert automatiquement :

- Mega Ring après Mortimer ;
- disponibilité de la Méga joueur avec une pierre compatible ;
- commande Méga ;
- Méga des boss ;
- formes/talents pré- et post-Méga.

La prochaine étape est une **validation en ROM**, pas une nouvelle réécriture du système.

### Évolutions

Le bloc évolution est **terminé**.

Règle Tactica :

- échange simple → **Fil Liaison** ;
- échange + objet tenu → objet officiel tenu + **Fil Liaison** ;
- mécanique officielle impossible à reproduire proprement → **Fil Liaison** uniquement pour les cas validés ;
- toute méthode officielle réalisable reste officielle, sans seuil de niveau ou raccourci inventé.

État protégé :

- aucun `EVO_TRADE` actif ;
- aucun `IF_MIN_LEVEL` artificiel sur les évolutions par objet ;
- méthodes non-échange officielles restaurées ;
- cas impossibles validés migrés ;
- Pokédex régénéré depuis le runtime ;
- synchroniseur `tools/sync_tactica_species_evolutions.py --check` intégré à la CI.

Ne pas rouvrir ce bloc sans régression démontrée.

### Rival / Rocket

Rocket reste intégré côté sources/runtime et doit surtout être observé dans la prochaine ROM candidate.

Le correctif de progression **Rival est intégré** :

- premier duel : 1 Pokémon niveau 17 ;
- aucun combat scénario après le badge 1 ;
- après Hector : 4 Pokémon, niveaux 29–32 ;
- Tour Cendrée : 6 Pokémon, niveaux 35–38 ;
- combats suivants : 61–64, 65–67, puis 95/95 ;
- source canonique, parties runtime, générateur, caps et validateurs sont synchronisés.

Le bloc reste `[PARTIEL]` tant que les combats n'ont pas été observés dans une ROM candidate fraîche.

## Priorités actuelles

### BLOC TERMINÉ — progression Rival

La progression Rival a été réintégrée sans reprendre les autres changements de l'ancienne branche Codex. Ne pas rouvrir ce bloc sans défaut démontré par les tests ou le playtest ROM.

### PRIORITÉ 2 — ROM candidate propre

Une fois les correctifs prioritaires fusionnés :

```bash
git fetch origin
git switch integration/v1
git pull --ff-only origin integration/v1
git rev-parse HEAD
make clean
make hns -j4
```

La ROM testée doit être celle générée depuis ce SHA exact.

Un simple `git pull` ne met pas à jour une ancienne ROM déjà compilée.

### PRIORITÉ 3 — playtest owner

Le playtest owner est effectué manuellement par l'owner.

Il doit valider en priorité :

- intro Chen / avertissements / second starter ;
- UI réellement visible ;
- Difficulty et persistance ;
- Rival / Rocket ;
- Méga boss et joueur ;
- évolutions / Fil Liaison sur quelques témoins ;
- progression Kanto, Jeannine et deuxième Ligue.

Ne consigner comme défaut que ce qui est réellement reproduit sur la nouvelle candidate.

## IA — à tester avant toute correction

**Ne pas modifier l'IA pour l'instant.**

La version actuelle de l'IA Difficile n'a pas encore reçu son premier vrai playtest owner complet dans sa forme actuelle.

Les anciens retours sur Mimiqui, Provoc de Cornèbre ou Téraclope sont uniquement des **témoins historiques à surveiller**. Ils ne doivent plus être considérés comme bugs actuels tant qu'ils ne sont pas reproduits sur la prochaine ROM candidate.

Après ce premier test HARD :

- si le comportement est correct → ne rien changer ;
- si quelques défauts sont reproduits → correctifs ciblés et génériques ;
- si le comportement global est insuffisant → réévaluer le bloc IA dans son ensemble.

Ne pas lancer d'audit IA avant ce playtest.

## Non prioritaire

### Écran titre

Défaut visuel connu mais non bloquant :

- le titre « Pokémon Tactica » n'est pas parfaitement centré sur l'écran titre avant la sélection de partie / New Game.

Ce point est **non prioritaire** et ne doit pas retarder la prochaine ROM candidate ni le playtest des correctifs importants.

Lorsqu'il sera traité, corriger uniquement le positionnement du titre sans refaire l'écran.

## Wiki

La PR #29 reste la baseline visuelle/structurelle.

Ne pas restaurer une ancienne version et ne pas refaire le style global.

Les données doivent être modifiées dans leurs sources canoniques puis régénérées. Ne jamais corriger manuellement un fichier généré lorsqu'un synchroniseur existe.

Les méthodes d'évolution affichées doivent rester alignées sur `EVOLUTIONS.md` et le runtime réellement compilé.

## Politique de validation

Pendant le développement :

- tests ciblés du domaine ;
- générateurs `--check` concernés ;
- build HnS si runtime/code modifié.

Avant merge vers `integration/v1` :

- validateurs pertinents verts ;
- synchronisations vertes ;
- build/smoke tests appropriés ;
- CI verte.

Le playtest owner final n'est pas requis pour merger une correction automatisée propre, mais aucun test mGBA ne doit être déclaré effectué s'il ne l'a pas été.

## Compte rendu attendu

Toujours fournir factuellement :

- SHA de départ et SHA final ;
- branche / PR ;
- fichiers modifiés ;
- cause trouvée ;
- données/générateurs touchés ;
- tests et CI ;
- contrôles ROM encore non effectués.
