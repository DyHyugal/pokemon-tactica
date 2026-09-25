# Boutiques et assistant

## PNJ capacités / CT

Rendre achetables **toutes les capacités utilisables dans le build**, notamment Boutefeu et Colère, même si elles n'ont pas d'objet CT natif. Une interface de catalogue de Move IDs est acceptable : prix, catégories, compatibilité du Pokémon, confirmation et apprentissage. L'achat n'autorise pas un Pokémon à apprendre hors de son movepool. Auditer la totalité des Move IDs actifs, les capacités internes/inutilisables et les limites d'interface avant de coder. Les CT existantes restent accessibles via ce PNJ.

## PNJ objets

Proposer les objets utiles qui ne sont pas déjà vendus par les marchands habituels : évolution, objets tenus et de combat, baies et préparation stratégique selon ce qui existe dans le build. Exclure le stock normal de soins, statut, Balls, Repousse, Corde Sortie et équivalents déjà couverts, ainsi que Key Items, objets de quête/progression et tout objet qui produirait une incohérence scénaristique. La liste exhaustive est **à générer par inventaire du code et des boutiques existantes** ; ne pas traiter un ancien sous-ensemble de boutique comme liste complète.

## Méga-Gemmes

Inventorier les Méga-Gemmes réellement supportées et rechercher un vendeur dédié. S'il n'existe pas, créer un PNJ Méga-Gemmes à côté du PNJ capacités/CT. Les gemmes déjà disponibles ailleurs ne doivent pas bloquer un accès cohérent aux Méga du jeu ; exclure celles sans forme jouable. Chaque Méga-Gemme vendue coûte exactement **3000 Pokédollars**. Tester le catalogue, l'emplacement, l'achat, le retrait de l'argent et la réception de l'objet dans la ROM.

## Entraînement

L'assistant centralisé permet EXP jusqu'au cap, IV, EV et remise à zéro, nature, talent et bonheur. Le prix/les restrictions ne doivent pas annuler l'objectif de stratégie sans grind.
