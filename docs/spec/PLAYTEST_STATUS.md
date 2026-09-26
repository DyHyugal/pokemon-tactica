# Statut de playtest — Pokémon Tactica

Ce fichier est la référence opérationnelle pour savoir **quelle ROM le propriétaire doit réellement tester**.

## Candidate owner actuelle

**Aucune candidate owner validée au 26 septembre 2026.**

Deux SHA sont suivis séparément afin de ne plus confondre état Git et état gameplay validé.

Dernier SHA `integration/v1` observé après la PR #20 :

```text
8cbdbeb41adfeb62d78d520200e53659fbb1441b
```

Dernier SHA gameplay/runtime servant encore de référence automatisée :

```text
04e805a318fd00b882df4cd6283808b9ae25b3b8
```

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

Ces points sont **à corriger ou à revalider avant de demander un nouveau playtest de progression** :

- sélecteur starter : position initiale et retour après annulation incohérents ; Élekid peut renvoyer sur `Retour` ;
- Summary : ancienne composition BG/tilemaps/windows encore présente, superpositions STATS/IV/EV et pages d'attaques ;
- menus custom des boutiques : rendu actuel non conforme à la charte rouge/noir ;
- HUD combat : healthboxes déjà assombries mais panneaux action/attaques encore non conformes au rendu final attendu ;
- premier rival : le template moteur reste niveau 5 ; contrat owner = starter seul mais niveau 17, avec évolution légale à ce niveau ;
- Route 36 : **ne pas retester l'état actuel**. Les sources fusionnées sont déjà connues fausses (27–30, cap de zone mal classé). Corriger d'abord à 14–17 max avec les stades correspondants, puis reconstruire la ROM ;
- Mortimer : le générateur peut encore produire Ectoplasma @ Ectoplasmite avec Shadow Tag sur la forme de base ; la Méga doit être corrigée ;
- Mega Ring : doit être remis après le badge 4, avant la portion vers le badge 5.

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
