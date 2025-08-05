def calculate_serial(username):
    """
    Calcule le numéro de série attendu pour un nom d'utilisateur donné
    """
    # Vérifier que le nom d'utilisateur a au moins 6 caractères
    if len(username) < 6:
        print("Erreur: Le nom d'utilisateur doit avoir au moins 6 caractères")
        return None

    # Calculer le numéro de série initial basé sur le 4ème caractère
    # (username[3] car l'indexation commence à 0)
    expected_serial = (ord(username[3]) ^ 4919) + 6221293

    print(f"Valeur initiale (4ème caractère '{username[3]}'): {expected_serial}")

    # Traiter chaque caractère du nom d'utilisateur
    for index, char in enumerate(username):
        # Vérifier les caractères non-imprimables
        if ord(char) < ord(' '):
            print(f"Erreur: Caractère non-imprimable détecté à la position {index}")
            return None

        # Mettre à jour le numéro de série
        old_serial = expected_serial
        expected_serial = expected_serial + ((ord(char) ^ expected_serial) % 1337)

        print(f"Index {index}, char '{char}' (ASCII {ord(char)}): {old_serial} -> {expected_serial}")

    return expected_serial

if __name__ == "__main__":
    test_username = input("Entrez le nom d'utilisateur: ").strip()
    serial = calculate_serial(test_username)
    if serial is not None:
        print(f"\nNom d'utilisateur: {test_username}")
        print(f"Numéro de série calculé: {serial}")
    else:
        print("Impossible de calculer le numéro de série")