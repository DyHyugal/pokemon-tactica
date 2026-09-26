# Rencontres V1

Sources canoniques :

- `data/spec/encounters_standard.json` : 405 tables standard ;
- `data/spec/encounters_special.json` : 4 Headbutt + 53 pools Safari ;
- `data/spec/encounter_access_caps.json` : premier accès réel par map/méthode.

## Contrat

Chaque table active contient quatre entrées réelles pondérées `30/30/30/10`.

Méthodes standard : `land_mons`, `water_mons`, `rock_smash_mons`, `fishing_mons`.

Les cannes/CS ouvrent une méthode mais ne modifient pas le scaling d’une zone déjà accessible. Le niveau cible vient du **premier accès réel à la zone**.

Contraintes :

- habitat cohérent ;
- stade d’évolution cohérent au niveau rencontré ;
- aucune famille starter avant badge 2 ;
- pas de légendaire/fabuleux ajouté comme encounter standard ;
- starters sauvages autorisés ensuite de façon rare et cohérente ;
- Route 36 reste `14–17`.

## Témoins canoniques early

### Route 31 — jour

`7–11`, herbe/sol :

- Wattouat 30 %
- Nirondelle 30 %
- Piafabec 30 %
- Tarsal 10 %

Wattouat doit rester disponible avant la première Ligue.

### Route 36 — jour

`14–17`, herbe/sol :

- Baggiguane 30 %
- Scorplane 30 %
- Nounourson 30 %
- Goupix 10 %

Scorplane est volontairement disponible early. Son occurrence tardive aux Tourb’Îles peut être réévaluée pendant l’audit global si elle occupe un slot utile à une espèce absente.

### Route 36 — autres méthodes

Toutes les méthodes de Route 36 partagent le même cap de premier accès et restent à `14–17`. Le déblocage ultérieur d’Éclate-Roc ne rehausse pas les niveaux.

## Couverture avant la première Ligue

Objectif owner : maximiser la variété réellement capturable avant la première Ligue.

Règle :

> Lorsqu’une espèce apparaît plusieurs fois avant la Ligue alors qu’une autre espèce reste uniquement post-Ligue, remplacer en priorité les doublons inutiles, sous réserve d’habitat, de progression, de niveau et de stade d’évolution cohérents.

Cette règle complète les contraintes précédentes et ne justifie jamais un remplissage aléatoire.

Audit courant du dataset standard après les corrections early :

- 479 espèces différentes sont utilisées dans les tables standard ;
- 409 sont présentes dans une table dont le premier accès est au plus tard au cap Ligue (`<= 67`) ;
- 70 restent uniquement dans des tables post-Ligue ;
- 152 espèces apparaissent plus d’une fois avant la Ligue.

Ces chiffres décrivent uniquement le dataset standard actuellement utilisé ; ils ne prétendent pas représenter l’intégralité des espèces compilées dans le moteur.

Le chantier de couverture globale reste donc **PARTIEL**. Les remplacements doivent être effectués par lots cohérents et validés, pas en mélange avec une correction de CI ou d’UI.

## Synchronisation

Ordre obligatoire après modification :

1. éditer le JSON canonique ;
2. `tools/sync_tactica_encounter_access.py` si les caps changent ;
3. `tools/sync_tactica_encounters.py` pour le runtime ;
4. `tools/validate_tactica_encounter_evolutions.py` ;
5. `tools/sync_tactica_localization.py` pour Localisations/Pokédex ;
6. relancer les modes `--check` concernés.

`sync_tactica_localization.py` synchronise désormais les niveaux **et les espèces** des 405 tables standard dans les pages FR/EN, reconstruit les index de recherche HTML et recalcule les compteurs Pokédex.

## Contrôle ROM

La cohérence JSON/runtime est automatisée, mais une candidate owner doit encore vérifier quelques zones témoins en ROM fraîche :

- Route 31 jour : Wattouat ;
- Route 36 jour : Scorplane et niveaux 14–17 ;
- au moins une méthode débloquée plus tard sur une ancienne zone pour confirmer l’absence de scaling artificiel.

Ne jamais déclarer cette observation faite sans test réel.
