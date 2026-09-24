[Accueil](Accueil.md) · [Guide de jeu](Guide-de-jeu.md) · [Changements](Changements.md) · [Boss & Conseils](Boss-et-Conseils.md) · [Pokédex](Pokedex.md) · [Localisations](Localisations.md) · [Crédits & Versions](Credits-et-Versions.md) · **[EN](../EN/Changes.md)**

# Changements

Cette page regroupe les différences principales entre Pokémon Heart & Soul et Pokémon Tactica sans multiplier les micro-pages.

## Systèmes

Tactica ajoute ou modifie notamment :
- HARD par défaut, NORMAL disponible ;
- nouvelles équipes Tactica dans **les deux difficultés** ;
- Level Cap dynamique ;
- Training NPC ;
- vitesse native x1/x2/x3/x4 ;
- réglages audio indépendants ;
- nouvelle configuration de partie ;
- sauvegarde des paramètres avec B = Save & Exit ;
- sélection de langue EN / FR prévue pour la V1.

## Starters et œuf d'Orme

Le choix initial comprend 30 starters répartis entre Feu, Eau, Plante, Électrik, Sol et Glace, plus Évoli.

L'œuf d'Orme devient une seconde sélection : l'espèce choisie est réellement contenue dans l'œuf. Les boosters de type et objets d'évolution nécessaires sont remis avec les starters. Évoli reçoit Mouchoir Soie, booster de son évolition et pierre correspondante.

## Pokémon

**26 modifications Pokémon** sont prévues dans la V1. Elles peuvent toucher type, statistiques, talents ou rôle.

Exemples verrouillés :
| Pokémon | Changement principal |
|---|---|
| Dracaufeu | Feu / Dragon |
| Typhlosion | Feu / Sol |
| Jungko | Plante / Dragon, orientation plus physique |
| Luxray | Électrik / Ténèbres |
| Moustillon / Mateloutre / Clamiral | Eau / Combat |
| Élekable | profil offensif et vitesse retravaillés |
| Hyporoi | profil physique renforcé |

La table exhaustive sera synchronisée avec les données compilées afin de ne pas publier de valeurs intermédiaires.

## Évolutions et learnsets

Evolution gate :
- base → stade 1 : niveau 16 ;
- base → final sans stade intermédiaire : niveau 30 ;
- stade 1 → final : niveau 36.

La V1 comprend **15 learnsets custom** et utilise le comportement GEN_7 prévu par le projet.

Correspondances importantes :
- **Aquatacle** = `MOVE_WAVE_CRASH`
- **Eau Revoir** = `MOVE_FLIP_TURN`

## Rencontres sauvages

La distribution suit la progression de l'histoire et exploite herbe, Surf, pêche, Rock Smash, horaires et autres méthodes disponibles.

Principes :
- tables principales basées sur quatre espèces et un poids cible 30 / 30 / 30 / 10 ;
- starters sauvages rares et progressifs ;
- formes finales surtout tardives ;
- aucun légendaire/fabuleux ajouté comme rencontre aléatoire ordinaire ;
- Safari conservé comme système dédié.

État intégré avant la passe finale : **405 tables standard + 4 Headbutt**. Le Safari comporte **53 pools** distincts, chacun à quatre espèces en 30/30/30/10, avec rotation des pools pendant une admission.

## Combats de boss

Les nouvelles compositions Tactica sont communes à NORMAL et HARD.

**NORMAL** : mêmes équipes/sets, scaling IV/EV NORMAL et IA NORMAL.

**HARD** : IV31, EV optimisés par rôle et IA HARD en plus des synergies prévues.

À partir de la troisième arène, les Champions utilisent des équipes complètes de six Pokémon. Les Méga apparaissent à partir de la quatrième arène.

Les dirigeants Rocket suivent une progression dynamique : **dernier cap canonique Champion/rival jalon +2 sur toute l'équipe** ; un boss Rocket ne sert jamais de référence au suivant.

Voir [Boss & Conseils](Boss-et-Conseils.md).

## Boutiques

TM Shop : **Catégorie → Type → CT** avec Support, Weak, Powerful et Ultimate.

Item Shop : **Evolution → Strategic → Other**.

Les boutiques spéciales ne remplacent pas la progression normale des Poké Marts.

## Interface et confort

La V1 conserve la base graphique HnS avec une passe d'harmonisation : battle UI, Settings, Summary, Pokédex, Shops, fenêtres, menus, textes, alignements, palettes et glitches visibles.

Une refonte graphique plus profonde reste réservée à une future V2.
