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
5. les documents métier réellement concernés
6. les `data/spec/*.json` concernés

Une branche/PR non fusionnée n’est jamais considérée comme l’état testable owner.

## Règle documentaire

Les docs actives décrivent uniquement l’état courant. Ne pas conserver plusieurs couches « décision initiale / correction / correction finale ». L’historique est dans Git.

Hiérarchie : `SOURCE_OF_TRUTH.md` → docs métier → JSON canoniques → runtime généré.

## Baseline à protéger

Ne pas recoder sans défaut démontré :

- vitesse native x1/x2/x3/x4 ;
- audio indépendant ;
- Shiny Rate ;
- évolutions solo ;
- sélection des 30 starters + Évoli ;
- curseur initial du sélecteur et retour après annulation ;
- œuf d’Orme distinct du starter principal ;
- rival fixe par archétype, premier combat niveau 17, progression 1→3→4→6 ;
- rosters Rival/Rocket courants ;
- Méga Rival après badge 4 ;
- Méga Rocket uniquement FINAL ;
- Archer = Méga-Sharpedo sans Abri préalable ;
- talents pré-Méga légaux ;
- Mega Ring après Mortimer ;
- règle du premier accès réel pour les encounters ;
- Route 36 à 14–17 ;
- 405 tables standard synchronisées ;
- baseline wiki fusionnée via PR #29, y compris Routes/Villes.

## État encounters courant

- quatre slots `30/30/30/10` ;
- Scorplane : Route 36 jour, early ;
- Wattouat : Route 31 jour ;
- les localisations/wiki doivent être régénérés depuis les JSON, espèces **et** niveaux ;
- la couverture du dataset standard pré-Ligue est complète : 479 espèces utilisées sur 479 sont disponibles avant/à la première Ligue. Conserver cette couverture ; ne pas réintroduire d’espèce uniquement post-Ligue lors d’une future passe.

## État boss/Méga courant

- exactement une Méga par Champion à partir de Mortimer ;
- unicité globale Champions / Conseil 4 / Rival / Rocket ;
- Jeannine = Méga-Kravarech @ Dragalgite ;
- forme de base légale avant Méga, talent Méga appliqué par la transformation.

## Priorités UI V1

1. vérifier en ROM fraîche le Summary structurel déjà intégré ;
2. vérifier en ROM le HUD combat, dont la suppression de la plaque blanche à gauche de la barre HP ;
3. vérifier en ROM les boutiques rouge/noir et la lisibilité de la sélection ;
4. fenêtres custom / polish restant.

Un validateur statique ne remplace jamais une observation ROM pour ces points.

## Wiki

La passe owner de la PR #29 est la baseline. Ne pas restaurer une ancienne version et ne pas refaire le style global.

Les corrections data doivent passer par les sources canoniques puis les générateurs wiki. Le synchroniseur Localisations/Pokédex doit rester idempotent et la recherche HTML doit refléter les espèces canoniques FR/EN.

## Politique de validation

Pendant le développement : tests ciblés du domaine + générateurs `--check`.

Avant merge vers `integration/v1` :

- validateurs pertinents verts ;
- synchronisations `--check` vertes ;
- build/smoke tests si code ou runtime modifié ;
- CI verte.

Le playtest owner final n’est pas requis pour merger une correction automatisée propre. Les vérifications ROM restantes sont consignées comme telles.

Avant une candidate owner : CI verte sur le SHA exact, `make clean && make hns -j4`, puis checklist ROM de `PLAYTEST_STATUS.md`.

## Compte rendu attendu

Toujours fournir factuellement :

- SHA de départ et SHA final ;
- branche/PR ;
- fichiers modifiés ;
- données/générateurs corrigés ;
- tests et CI ;
- synchronisations/idempotence ;
- contrôles ROM encore non effectués.

Ne jamais déclarer un test mGBA effectué s’il ne l’a pas été.
