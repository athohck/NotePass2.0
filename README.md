# NotePass 2.0

Gestionnaire de mots de passe chiffré en C — version 2


## Description

NotePass 2.0 est un gestionnaire de mots de passe en ligne de commande écrit en C. Chaque utilisateur possède son propre compte sécurisé par hachage SHA-256, et son coffre personnel chiffré avec XOR. Version améliorée de NotePass 1.0 avec un système multi-utilisateurs complet.

## Fonctionnalités

- **Création de compte** — ID + mot de passe (11 caractères minimum) + clé de chiffrement, tout hashé en SHA-256
- **Connexion sécurisée** — vérification ID + mot de passe par comparaison de hash SHA-256
- **Coffre personnel** — fichier `id_Coffre.txt` unique par utilisateur, protégé par clé
- **Chiffrement XOR** — comptes et mots de passe stockés chiffrés, déchiffrés à la lecture

## Prérequis

- macOS (notification via `osascript`)
- OpenSSL (`brew install openssl`)
- GCC ou Clang

## Compilation
```bash
gcc notePass2.0.c -o notePass2.0 -I/opt/homebrew/include -L/opt/homebrew/lib -lssl -lcrypto

```

## Structure des fichiers

| Fichier | Contenu |
|---|---|
| `Compte.txt` | Hash SHA-256 des identifiants (id, mdp, clé) |
| `id_Coffre.txt` | Coffre chiffré XOR de l'utilisateur |

## Différences vs NotePass 1.0

- Hachage SHA-256 (vs stockage en clair)
- Coffre unique par utilisateur (vs coffre global partagé)
- Vérification de la clé de chiffrement avant accès au coffre
- Validation de la longueur du mot de passe à la création

---

Projet d'apprentissage — AthoHck • 2026
```

Ensuite :
```
git add README.md
git commit -m "add README"
git push
