Yanis Mazouz Groupe 01 .
R1: Quelles sont les structures de données à utiliser ?
Matrices B, C, et A : Structures pour stocker les valeurs des matrices.
Tampon T : Tableau de structures Element pour stocker les résultats intermédiaires.
R2: Comment allez-vous protéger l'accès à ces données?
Mutex ('pthread_mutex_t mutex') et Conditions ('pthread_cond_t empty' et 'pthread_cond_t full') ils sont utilisés pour protéger l'accès au tampon T et les conditions 
pour la synchronisation entre producteurs et consommateurs.
R3: quels sont les risques?
Concurrence pour le tampon.
Problèmes de synchronisation.
Accès concurrents aux matrices.
