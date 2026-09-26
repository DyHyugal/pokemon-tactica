# Statut de playtest — Pokémon Tactica

Ce fichier est la référence opérationnelle pour savoir **quelle ROM le propriétaire doit réellement tester**.

## Candidate owner actuelle

**Aucune candidate owner validée au 26 septembre 2026.**

Deux SHA sont suivis séparément afin de ne plus confondre état Git et état gameplay validé.

Dernier SHA `integration/v1` intégré après les PR #22 CORE et #23 Encounters :

```text
96f0b2c86da3d7bdfc8af47309bf366656351f0d
```

Ce SHA contient les corrections CORE et Encounters automatisées. Il n'est **pas encore une candidate owner** : le bloc UI/UX reste à corriger avant la prochaine recette globale.

Le SHA gameplay ci-dessus contient beaucoup d'avancées automatisées (encounters, rival dynamique, boss/Rocket, balance, shops, caps), mais il **ne doit pas être présenté comme la RC / build finale des blocs CORE + encounters + UI**, car plusieurs retours owner connus restent non implémentés ou non validés en ROM.

## Pourquoi un simple pull ne suffit pas

`*.gba` et `build/` sont ignorés par Git. Par conséquent :

- `git pull` met à jour le **code source** ;
- il ne remplace pas l'ancien `pokehns.gba` ;
- relancer mGBA sur l'ancien fichier après un pull revient à tester une ancienne build.

### Procédure de référence côté propriétaire

```bash
git fetch origin
git switch integration/v1
git pull --ff-only origin integration/v1
git rev-parse --short HEAD
make clean
make hns -j4
```

La ROM générée est :

```text
pokehns.gba
```

Avant de jouer, vérifier que sa date de modification correspond bien à la compilation qui vient d'être faite. Dans mGBA, ouvrir explicitement **ce fichier**, et non une copie plus ancienne placée ailleurs.

Pour les rebuilds quotidiens sans doute sur les artefacts, `make hns -j4` suffit généralement ; `make clean` est exigé pour une candidate owner ou lorsqu'on soupçonne une ancienne ROM / anciens assets.

## Écarts connus avant la prochaine candidate

Les anciens défauts source « rival niveau 5 », Route 36 27–30, talent Méga illégal de Mortimer, Mega Ring absent après badge 4 et bugs logiques du sélecteur ont été corrigés par les PR #22/#23 et ne doivent plus être présentés comme non implémentés. Ils restent des **témoins ROM à revalider** sur la future candidate.

Les écarts encore réellement à corriger avant cette candidate sont UI/UX :

- Summary : recomposition BG/tilemaps/windows, lisibilité STATS/IV/EV et remplacement possible de `CONTEST MOVES` par une page IV/EV ;
- boutiques custom : fond rouge, texte noir, sélection lisible sans grand surlignage blanc ;
- HUD combat : healthboxes déjà assombries, panneaux action/attaques encore à harmoniser noir/rouge/gris ;
- polish visuel du sélecteur starter et des fenêtres custom à confirmer pendant la recette.

Les témoins gameplay à **revalider**, et non à recoder sans défaut démontré, sont : curseur/annulation starter, premier rival niveau 17, combats Rival/Rocket progressifs, Méga de Mortimer, réception/utilisation du Mega Ring et Route 36 14–17.

## Règle de publication d'une prochaine candidate

Une prochaine candidate n'est annoncée que lorsque :

1. toutes les corrections annoncées comme incluses sont fusionnées dans `integration/v1` ;
2. la CI Tactica est verte sur **le SHA exact d'intégration** ;
3. les tests/générateurs/validateurs du périmètre passent ;
4. `make clean && make hns -j4` passe ;
5. ce fichier est mis à jour avec le SHA exact et une checklist de test manuelle ;
6. aucun défaut déjà connu et non corrigé n'est redemandé au propriétaire comme s'il s'agissait d'un nouveau test.

## Checklist de la prochaine run owner

Quand une candidate sera déclarée prête, vérifier au minimum :

- nouveau jeu → sélecteur starter : curseur en haut ; annulation = retour sur le même Pokémon ;
- premier rival : un seul Pokémon, niveau 17, stade légal ;
- Route 36 : quatre slots conformes aux familles prévues, **niveaux 14–17 max** et stades d'évolution cohérents ;
- boutiques custom : fond rouge, texte noir, sélection lisible ;
- combat : healthboxes + menu d'action + menu des attaques conformes à la charte ;
- Summary : toutes les pages lisibles, sans superposition ; IV/EV utilisables ;
- progression jusqu'à Mortimer : boss Méga sans assert et Méga réellement déclenchée ;
- après badge 4 : obtention du Mega Ring puis utilisation d'une Méga joueur avant le badge 5.
