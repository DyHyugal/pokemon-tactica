# Boss, Méga et IA

Le roster de référence est `data/spec/bosses.json`. Il reprend le dataset de boss v8 avec les décisions ultérieures appliquées explicitement. Un niveau `null` signifie dynamique, et non manquant. Toute divergence avec les fichiers compilés doit être signalée dans l'audit avant correction.

## Structure et difficulté

Albert 3, Hector 4, Blanche et après 6 Pokémon. Même roster, moves, talents, objets, natures et niveaux en NORMAL/HARD ; HARD : IV 31 pour boss fixes, EV au plus 252 par statistique et 510 au total, IA stratégique équitable. NORMAL conserve l'optimisation native. Pour les attaquants, privilégier nature Vitesse ou attaque utile en réduisant l'attaque inutilisée, sauf justification de rôle. Au plus deux soins s'ils sont prévus, du type d'objet effectivement accessible au stade concerné.

L'IA peut gérer suicide lead, climat/terrain, murs, pivots, setup et plusieurs carries. Les hazards ne sont pas obligatoires dans toutes les équipes. Elle ne lit pas des informations cachées du joueur et ne prédit pas omniscientement son choix. Gestion des objets Choix, capacités de statut, météo, écrans et bascule vers un autre rôle lorsqu'un plan ne marche plus. La logique détaillée s'appuie sur les rôles des équipes et les tests de combat, sans donner à HARD un roster différent.

## Identités confirmées

Frédo : Feunard d'Alola, Momartik, Blizzaroi, Glaivodo, Galvagla, Cochignon. Feunard d'Alola pose Neige et Voile Aurore puis peut se retirer ; Cochignon Évoluroc/Isograisse/Malédiction. **Pas de Mammochon.** Jasmine : Noacier, Gromago, Corvaillus, Pondralugon, Magnézone, **Méga-Galeking**, sans tempête de sable. Pierre garde Méga-Steelix. Ondine Pluie et Méga-Staross si effectivement jouable ; Major Bob Champ Électrifié et Méga-Raichu si jouable ; Erika Champ Herbu ; Auguste Soleil ; Morgane Distorsion ; Jeannine Poison/contrôle ; Blue hyper offense. Clément Méga-Gardevoir, Morgane Méga-Alakazam. Rocket : Proton hyper offense/Poison ; Petrel nuisance ; Ariana contrôle/setup ; Archer offense équilibrée. Les autres équipes et membres du Conseil 4 sont précisés par le JSON, et non supprimés faute d'être énumérés ici.

Les Méga des boss majeurs doivent être uniques ; en cas de doublon, le personnage le plus lié au Pokémon garde sa Méga. L'autre reçoit une Méga disponible et cohérente avec son type, climat, terrain et vitesse. Le set de Jasmine doit partir d'un **vrai set de Méga-Galeking**, pas d'un Galeking normal renommé. Candidat de référence : Aggronite, Malédiction, Tacle Lourd, Repos, Blabla Dodo, nature Prudente, EV 252 PV / 4 Déf / 252 Déf. Spé, talent après Méga selon le build. Ce set est un set de Méga-Galeking documenté ; aucun prétendu set critique populaire n'a été identifié. Vérifier les noms, capacités, talent et objet dans le moteur avant de le figer.

Les Simiabraz de Chuck et d’Aldo utilisent Acrobatie. Méga-Altaria garde Retour et reçoit le bonheur maximal dans les données d’équipe afin d’en garantir la puissance maximale en combat. Cette valeur interne n’a pas à être exposée dans le wiki joueur.

## Points du dataset corrigés

Voltali du Maître en rematch : Modeste, Vive-Attaque + Change Éclair, rôle spécial. Hyporoi garde Lentilscope. Corboss avec Orbe Vie là où le roster l'attribue. Magnézone spécial : Rayon Signal au lieu de Big Splash. Noadkoko spécial : Pouvoir Antique au lieu de Poudre Dodo. Rocket prend la plage de la dernière arène vaincue +2, **jamais** le Rocket précédent +2.

## Match retour du Maître

Au niveau 100 : Voltali conserve son set ; Méga-Dracaufeu X reprend le set du premier match ; Hydragon est Rigide, Prognathe, Mouchoir Choix, DPS physique, avec Branchicrok / Psycho-Croc / Mâchouille / Draco-Griffe ; Miascarade conserve son set avec Bandeau Choix ; Exagide est Rigide, DPS physique, Restes, avec Danse Lames / Tête de Fer / Ombre Portée / Bouclier Royal ; Carchacrok conserve les capacités, talent, nature et EV du premier match avec Orbe Vie afin de préserver l’unicité des objets.

## Coordination climat / terrain HARD

La stratégie commune infère un plan depuis les talents et capacités de sa propre équipe, puis utilise les calculs de dégâts et de hazards du moteur. Le poseur transmet le terrain/climat à un bénéficiaire dès qu'il est actif ; une capacité super efficace sans KO ne suffit pas à le retenir. Un KO immédiat conservateur reste une raison valable d'attaquer. Un poseur de neige peut installer Voile Aurore avant la transmission. Quand le terrain/climat disparaît ou est remplacé, un poseur disponible est prioritaire sur un nouvel attaquant, y compris après un KO. Un poseur à talent déjà sur le terrain doit sortir puis revenir pour réactiver son talent. Le relais ne consomme pas volontairement le dernier tour de l’effet. Pour le rival Électrik, Wattapik est le poseur automatique et Salarsen (Champ Électrifié) le poseur manuel de secours ; Raichu d’Alola et Paume-de-Fer sont les bénéficiaires spécialisés. Sous terrain actif, les poseurs restent en réserve après leur travail.

Les bénéficiaires doivent être vivants, profiter de l'effet, et ne pas être mis KO à l'entrée par les hazards ou les dégâts connus lors d'un switch volontaire. L'ace est conservé tant qu'un autre bénéficiaire reste disponible. Sans bénéficiaire ou sans poseur utilisable, les décisions normales du moteur reprennent. Le mode NORMAL conserve sa logique ; HARD n'active ni omniscience, ni connaissance de l'équipe cachée, ni lecture/prédiction du choix du joueur. Les types visibles, capacités/objets révélés et hypothèses STAB du moteur restent disponibles.

Versions amont examinées : RHH/pokeemerald-expansion (`7b95be15d84a053948791ce91e5dec0b168947a5`) et pokemonhns-development/pokehns-expansion (`167aa6d537b109bb229c231ddce4616974c4da71`). Leur moteur gère les bénéfices de climat/terrain et les calculs de switches ; aucune orchestration complète du cycle poseur/bénéficiaire/restauration n'a été identifiée dans les sources examinées. La coordination Tactica s'appuie sur ce moteur, sans remplacer l'ensemble de l'IA.
