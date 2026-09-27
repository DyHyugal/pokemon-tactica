# Évolutions — règle canonique et audit HnS

État : décision owner du 27 septembre 2026.

Ce document définit la règle d'évolution de Pokémon Tactica et consigne l'audit des dérives héritées de HnS. Il doit servir de référence avant toute correction du runtime, des données générées ou du wiki.

## 1. Source de référence

La référence fonctionnelle du projet pour les méthodes d'évolution est **Poképédia**, en prenant la **méthode la plus récente applicable en 9G**.

Principes :

- partir de la méthode officielle 9G ;
- ne pas reprendre une ancienne méthode uniquement parce qu'elle existait dans une génération précédente ;
- ne pas ajouter de niveau minimum, d'objet de substitution ou de condition maison sans décision owner explicite ;
- lorsqu'une mécanique officielle est réalisable dans Tactica, elle doit être conservée telle quelle ;
- lorsqu'une mécanique officielle repose sur un échange ou est impossible à reproduire proprement dans la ROM, appliquer les règles de substitution définies ci-dessous.

Cette règle remplace les adaptations historiques HnS incompatibles avec ce document.

## 2. Nouvelle règle Tactica

### 2.1 Évolution par échange simple

**Règle : utiliser un Fil Liaison.**

Exemples de familles concernées :

- Kadabra -> Alakazam ;
- Machopeur -> Mackogneur ;
- Gravalanch -> Grolem ;
- Spectrum -> Ectoplasma ;
- Géolithe -> Gigalithe ;
- Ouvrifier -> Bétochef ;
- Brocélôme -> Desséliande ;
- Pitrouille -> Banshitrouye ;
- Carabing / Escargaume lorsque leur évolution officielle dépend de leur échange mutuel.

Aucun niveau minimum supplémentaire ne doit être ajouté si la méthode officielle n'en exige pas.

### 2.2 Évolution par échange avec objet tenu

**Règle : le Pokémon doit tenir l'objet officiel requis, puis le joueur utilise un Fil Liaison.**

Le Fil Liaison remplace uniquement l'action d'échanger. L'objet exigé reste celui de la méthode officielle.

Exemples :

- Rhinoféros tenant Protecteur + Fil Liaison -> Rhinastoc ;
- Téraclope tenant Tissu Fauche + Fil Liaison -> Noctunoir ;
- Élektek tenant Électriseur + Fil Liaison -> Élekable ;
- Magmar tenant Magmariseur + Fil Liaison -> Maganon ;
- Onix tenant Peau Métal + Fil Liaison -> Steelix ;
- Insécateur tenant Peau Métal + Fil Liaison -> Cizayox ;
- Hypocéan tenant Écaille Draco + Fil Liaison -> Hyporoi ;
- Porygon tenant Améliorator + Fil Liaison -> Porygon2 ;
- Porygon2 tenant CD Douteux + Fil Liaison -> Porygon-Z ;
- Têtarte tenant Roche Royale + Fil Liaison -> Tarpaud ;
- Ramoloss tenant Roche Royale + Fil Liaison -> Roigada ;
- Barpau tenant Bel'Écaille + Fil Liaison -> Milobellus ;
- Coquiperl tenant Dent Océan / Écaille Océan + Fil Liaison -> Serpang / Rosabyss ;
- Fluvetin tenant Sachet Senteur + Fil Liaison -> Cocotine ;
- Sucroquin tenant Chantibonbon + Fil Liaison -> Cupcanaille.

Aucun seuil de niveau artificiel ne doit être ajouté.

> La consommation exacte de l'objet tenu au moment de l'évolution relève de l'implémentation runtime ; ce document impose uniquement qu'il soit requis et tenu lors de l'utilisation du Fil Liaison.

### 2.3 Évolution officiellement impossible à reproduire proprement dans Tactica

**Règle : utiliser un Fil Liaison.**

Cette catégorie sert uniquement aux mécaniques qui ne peuvent pas être reproduites proprement dans le moteur ou l'expérience Tactica.

Exemples explicitement validés par l'owner :

- Sépiatop -> Sépiatroce ;
- Superdofin / la lignée de Dofin lorsque la condition multijoueur officielle n'est pas disponible ;
- autres cas équivalents identifiés lors de l'audit technique.

Il ne faut pas inventer une pierre, un niveau, un objet détourné ou une condition arbitraire pour ces cas.

## 3. Conséquences immédiates sur les cas déjà discutés

### Scalproie -> Scalpereur

La route officielle 9G doit être conservée lorsqu'elle est techniquement réalisable :

- vaincre trois Scalproie correspondant à la condition officielle liée à l'Emblème du Général ;
- puis gagner un niveau.

HnS/Tactica contient actuellement une route alternative via Roche Royale + niveau minimum. Cette route est une adaptation maison et doit être supprimée si la mécanique officielle est utilisable.

### Rhinoféros -> Rhinastoc

Ancienne décision temporaire : Protecteur + niveau 42.

**Nouvelle cible : Rhinoféros tient Protecteur + utilisation du Fil Liaison.**

Le niveau 42 n'est plus une condition d'évolution.

### Mélancolux -> Lugulabre

**Pierre Nuit uniquement.**

La contrainte HnS de niveau minimum 36 doit disparaître.

### Lampéroie -> Ohmassacre

**Pierre Foudre uniquement.**

La contrainte HnS de niveau minimum 36 doit disparaître.

### Téraclope -> Noctunoir

**Téraclope tient Tissu Fauche + utilisation du Fil Liaison.**

La contrainte HnS de niveau minimum 36 doit disparaître.

### Évoli et `data/spec/starters.json`

Les préférences d’Évoli présentes dans `data/spec/starters.json` proviennent de l’ancienne implémentation et peuvent encore fournir des pierres de raccourci à certaines évolitions.

**Ces préférences ne sont plus normatives pour la méthode d’évolution.** Évoli suit la même règle que toutes les autres espèces :

- conserver la méthode officielle 9G lorsqu’elle est réalisable ;
- n’utiliser Fil Liaison que si une mécanique tombe réellement dans la règle 2.3 ;
- supprimer lors de la migration les pierres/raccourcis qui contredisent la méthode officielle ;
- adapter ensuite les objets remis par le sélecteur starter/œuf afin qu’ils restent utiles et cohérents, sans redéfinir la méthode d’évolution.

## 4. Résultats de l'audit HnS / Tactica

### 4.1 Origine principale de la dérive

Les déclarations d'évolution importées dans Tactica correspondent à la production HnS utilisée comme base.

Le commit HnS :

- **`b7ea13f0ff0088edb8382c2e012e9e511119d6be`**
- message : **`Gate item evolutions by family stage`**
- date : **23 septembre 2026**

a introduit une condition générique `IF_MIN_LEVEL` sur un grand nombre d'évolutions par objet.

Audit de cette passe :

- **125 routes** ont reçu une nouvelle condition de niveau minimum ;
- seulement **23 routes** concernent une évolution dont la lignée officielle dépend réellement d'un échange ;
- **102 routes** sont hors échange ;
- ces 102 routes hors périmètre touchent **67 Pokémon sources**.

Conclusion : la règle `niveau 16 / 30 / 36 + objet` a été appliquée beaucoup trop largement et ne correspond pas à la nouvelle règle Tactica.

### 4.2 Pokémon hors échange affectés par les seuils artificiels

Les sources suivantes ont reçu au moins une contrainte `IF_MIN_LEVEL` sur une évolution qui n'était pas une évolution par échange :

`APPLIN`, `BISHARP`, `CAPSAKID`, `CETODDLE`, `CHARCADET`, `CHARJABUG`, `CLEFAIRY`, `COTTONEE`, `CRABRAWLER`, `DARUMAKA_GALAR`, `DIPPLIN`, `DOUBLADE`, `DURALUDON`, `EELEKTRIK`, `EEVEE`, `EXEGGCUTE`, `FLABEBE_WHITE`, `GLIGAR`, `GLOOM`, `GROWLITHE`, `GROWLITHE_HISUI`, `HAPPINY`, `HELIOPTILE`, `JIGGLYPUFF`, `KIRLIA`, `KUBFU`, `LAMPENT`, `LOMBRE`, `MAGNETON`, `MILCERY`, `MINCCINO`, `MISDREAVUS`, `MUNNA`, `MURKROW`, `NIDORINA`, `NIDORINO`, `NOSEPASS`, `NUZLEAF`, `PANPOUR`, `PANSAGE`, `PANSEAR`, `PETILIL`, `PIKACHU`, `POLIWHIRL`, `POLTCHAGEIST_ARTISAN`, `POLTCHAGEIST_COUNTERFEIT`, `ROSELIA`, `SANDSHREW_ALOLA`, `SCYTHER`, `SHELLDER`, `SINISTEA_ANTIQUE`, `SINISTEA_PHONY`, `SKITTY`, `SLOWPOKE_GALAR`, `SNEASEL`, `SNEASEL_HISUI`, `SNORUNT`, `STARYU`, `SUNKERN`, `TADBULB`, `TOGETIC`, `URSARING`, `VOLTORB_HISUI`, `VULPIX`, `VULPIX_ALOLA`, `WEEPINBELL`, `YAMASK_GALAR`.

Ces seuils ne doivent pas être conservés par défaut. La méthode officielle 9G de chaque lignée doit être restaurée, sauf si elle tombe dans les règles Fil Liaison de ce document.

### 4.3 Routes d'échange touchées par cette passe

Les sources suivantes appartiennent réellement au périmètre des évolutions par échange. Elles sont désormais migrées vers la règle Fil Liaison dans le runtime Tactica, au lieu des anciens seuils/raccourcis HnS :

- `BOLDORE`
- `CLAMPERL`
- `DUSCLOPS`
- `ELECTABUZZ`
- `FEEBAS`
- `GURDURR`
- `KARRABLAST`
- `MAGMAR`
- `ONIX`
- `POLIWHIRL`
- `PORYGON`
- `PORYGON2`
- `RHYDON`
- `SCYTHER`
- `SEADRA`
- `SLOWPOKE`
- `SPRITZEE`
- `SWIRLIX`

Cette liste correspond aux sources impactées par la passe HnS auditée. La migration runtime couvre également les autres évolutions par échange actives du Pokédex Tactica ; un test global interdit désormais toute route `EVO_TRADE` résiduelle.

## 5. Autres incohérences identifiées pendant l'audit

Les anomalies suivantes ne sont pas de simples seuils `IF_MIN_LEVEL` et doivent être revérifiées contre Poképédia 9G avant implémentation :

### Scalpereur

Une route alternative **Roche Royale** a été ajoutée alors que la mécanique officielle utilise l'Emblème du Général et une condition de combat avant le gain de niveau.

### Verpom

HnS a ajouté des alternatives génériques à base de pierres pour certaines branches :

- Pierre Plante ;
- Pierre Soleil ;
- Pierre Ovale.

Les objets/méthodes officiels de la 9G doivent rester la référence.

### Pomdramour -> Pomdorochi

Une route **Pierre Éclat** a été ajoutée alors que la mécanique officielle repose sur la condition de capacité correspondante puis un gain de niveau.

### Théffroi et Poltchageist

Des pierres ou objets génériques ont été ajoutés comme raccourcis alternatifs. Les objets officiels propres à ces lignées doivent être restaurés.

### Crèmy -> Charmilly

Des pierres d'évolution ont été ajoutées pour plusieurs formes. La mécanique officielle de Charmilly doit rester la référence lorsqu'elle est techniquement réalisable.

### Duralugon -> Pondralugon

Une route via **Améliorator** a été ajoutée. La méthode officielle 9G doit être conservée.

### Wushours

Des raccourcis via Griffe Rasoir / Pierre Eau ont été ajoutés. Les objets officiels de la lignée doivent rester la référence.

### Charbambin

Des raccourcis Pierre Feu / Pierre Nuit ont été ajoutés. Les armures officielles doivent rester la référence.

### Tutafeh de Galar -> Tutétékri

Une route Bloc de Tourbe a été ajoutée alors qu'elle n'appartient pas à la mécanique officielle de cette lignée.

Si la mécanique officielle n'est pas reproduisible proprement dans Tactica, ce cas doit utiliser **Fil Liaison** conformément à la règle 2.3, et non un objet arbitraire.

### Ursaking Lune Vermeille

La forme Lune Vermeille ne doit pas être traitée comme une évolution standard d'Ursaring si la méthode officielle ne le prévoit pas.

### Mordudor -> Gromago

Une route alternative par bonheur a été observée. La mécanique officielle 9G doit être conservée lorsqu'elle est réalisable.

### Stalgamin -> Oniglali

Un changement HnS a abaissé l'évolution d'Oniglali du niveau officiel vers le niveau 30. Cette modification doit être annulée si elle n'est pas couverte par une décision owner explicite.

## 6. Règles de migration

Lors de la correction du runtime :

1. prendre Poképédia 9G comme référence pour chaque lignée ;
2. restaurer les méthodes officielles déjà réalisables dans le moteur ;
3. supprimer les `IF_MIN_LEVEL` artificiels ajoutés aux évolutions par objet ;
4. supprimer les routes alternatives utilisant des objets sans rapport avec la méthode officielle ;
5. convertir les échanges simples en **Fil Liaison** ;
6. convertir les échanges + objet en **objet officiel tenu + Fil Liaison** ;
7. convertir les mécaniques réellement impossibles dans Tactica en **Fil Liaison** ;
8. ne conserver aucune exception par Pokémon sans décision owner explicite ;
9. régénérer ensuite les données du wiki depuis les sources canoniques au lieu de corriger manuellement les fichiers générés.

## 7. Validation attendue

Ajouter des contrôles génériques afin d'éviter une nouvelle dérive :

- aucune route d'évolution non officielle ajoutée silencieusement ;
- aucune contrainte `IF_MIN_LEVEL` sur une évolution par objet sauf si la méthode officielle 9G exige réellement ce niveau ;
- toutes les évolutions par échange disposent d'une route Fil Liaison ;
- pour échange + objet, le bon objet officiel doit être tenu ;
- aucune route ancienne d'échange ne doit être présentée au joueur comme méthode principale si elle est remplacée par Fil Liaison ;
- les mécaniques impossibles remplacées par Fil Liaison doivent être listées explicitement ;
- le wiki doit afficher uniquement les méthodes réellement utilisables dans Pokémon Tactica.

## 8. Portée de ce document

Ce document décrit la **cible**. Il ne signifie pas que les évolutions actuelles du runtime ont déjà été corrigées.

À la date de création de ce fichier :

- l'audit a identifié la dérive ;
- la règle Fil Liaison est validée par l'owner ;
- les corrections runtime restent à appliquer ;
- les données générées et le wiki devront être régénérés après correction du code.
