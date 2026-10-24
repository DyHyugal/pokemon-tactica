# Progression et caps

La courbe des niveaux d'ace validée est : Albert 18, Hector 25, Blanche 32, Mortimer 38, Chuck 45, Jasmine 52, Frédo 57, Sandra 64, Clément/Conseil 4 67, Maître 70. Les niveaux précis des autres membres viennent du roster canonique. Le cap courant ne décroît jamais.

## Dresseurs ordinaires

Les dresseurs ordinaires HnS des routes, grottes, bâtiments, leurs revanches et les sbires suivent la zone et l'avancement réel du scénario. À la création de l'équipe, son plus petit niveau est relevé au maximum de son niveau écrit, du plancher de zone et du plancher de scénario ; les écarts entre ses membres sont conservés dans la borne haute de zone ou du jalon atteint, avec une limite à 100. Un niveau écrit déjà supérieur à cette borne n’est jamais abaissé. Cela empêche un ancien écart très large de fabriquer un adversaire au-dessus des prochains boss.

Le plancher de zone vient du plus haut minimum des tables de rencontres standard de la carte. Un intérieur sans table hérite du plus petit plancher connu de sa section géographique. Une zone inconnue n'invente pas de niveau. Le plancher de scénario est le plus haut minimum d'un roster d'arène, Rival ou Ligue déjà vaincu. Les drapeaux de Maître établissent aussi le palier de Ligue ; les admins Rocket dynamiques ne sont pas recalculés rétroactivement dans ce plancher pour éviter un nouveau cumul de +2.

Le premier duel du Rival à Ville Griotte compte dès sa victoire : le plancher des dresseurs ordinaires passe à **18**, sans changer le cap joueur ni sa progression vers Albert. À Kanto, chaque victoire individuelle contre Morgane, Erika ou Jeannine établit immédiatement le plancher **80**, même tant que le groupe de trois n'est pas terminé pour ouvrir le cap suivant. Ces victoires déjà enregistrées dans une ancienne sauvegarde sont prises en compte automatiquement.

Le niveau de l'équipe du joueur, le mode NORMAL/HARD et son option de cap n'influencent pas ce calcul. Les évolutions par niveau déterministes et les capacités explicites suivent le nouveau niveau ; aucune évolution conditionnelle n'est inventée. Champions, Conseil 4, Rival, admins Rocket, Red et rencontres spéciales gardent leurs règles propres. La politique et les exclusions sont dans `data/spec/trainer_levels.json` ; zones et éligibilité sont générées par `tools/sync_tactica_trainer_levels.py`.

## Rival

Le rival suit les jalons de préparation de la progression, pas les niveaux historiques du jeu d'origine. Il persiste un **archétype** puis utilise le starter fixe de cet archétype ; l'ancien tirage d'une espèce parmi cinq starters est obsolète.

- Premier combat : **1 Pokémon**, le starter rival, **niveau 18**, aligné sur Roucoups d'Albert et avec un stade d'évolution légal au niveau 18.
- Les combats suivants gardent la progression **1 -> 4 -> 6** puis restent à 6 ; après le badge 4, l'équipe complète peut utiliser sa Méga définie dans `ROSTERS_ROCKET_RIVAL.md`.
- Un rival peut devenir jalon canonique uniquement lorsqu'il est explicitement désigné dans la progression.

## Rocket

Chaque admin Rocket est un boss et devient un jalon de cap, au même titre qu'un Champion. Ses non-aces reprennent le minimum de l'équipe de l'arène déjà vaincue la plus avancée +2 ; son ace reprend le maximum +2. Exemple Blanche 29–32 → Rocket 31–34. L'option de cap joueur ne change jamais cette équipe : le minimum sert au cap strict, le maximum au cap normal.

Plusieurs admins sans nouvelle victoire d'arène partagent cette même plage. Ni un autre admin ni le Rival ne devient la référence de leur +2. Le cap effectif conserve tout palier supérieur déjà acquis. Après la deuxième Ligue, il reste à 100 avant et après Red.

Le parcours Johto suit le Guide : Mortimer → Chuck → Jasmine → repaire Rocket → Frédo → Tour Radio → Sandra. Les victoires dans un autre ordre ne doivent jamais abaisser le cap. Les tests lisent les équipes de production, pas une table de niveaux injectée.

## Méga-Évolution joueur

Après la victoire contre Mortimer :

1. remise du badge 4 ;
2. remise du **Mega Ring** avec le flow/jingle d'objet approprié ;
3. remise de la CT prévue ;
4. à la sortie de l'arène, le joueur doit déjà pouvoir Méga-évoluer.

Le déblocage ne doit pas attendre le badge 5 ni un autre événement.

## Légalité

Les Pokémon des champions et du rival respectent les niveaux d'évolution. Cochignon de Frédo est un choix intentionnel. Pour les boss Méga, la forme de base doit avoir un talent légal avant transformation ; le talent Méga est obtenu par la transformation réelle.

## Kanto et deuxième Ligue

Après le Maître niveau 70, les caps progressent par paliers de cinq. Major Bob ouvre Kanto au niveau 75. Morgane, Erika et Jeannine partagent le palier 80 car leurs Arènes sont accessibles dans le même mouvement. Ondine suit au niveau 85, Pierre au niveau 90, Auguste au niveau 95, puis Blue et la deuxième Ligue au niveau 100. Le groupe central ne libère le palier suivant qu’après les trois victoires ; le cap ne redescend jamais. Le rival conserve sa logique propre.
