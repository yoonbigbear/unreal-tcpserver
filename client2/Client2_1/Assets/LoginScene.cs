using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class LoginScene : MonoBehaviour
{
    public Button create_account;
    public Button login;

    public Button select_character;
    public Button create_character;

    public TMP_InputField id;
    public TMP_InputField password;

    public TMP_InputField character_id;
    public TMP_InputField nickname;

    public Network network;

    // Start is called before the first frame update
    void Start()
    {
        create_account.onClick.AddListener(OnCreateAccount);
        login.onClick.AddListener(OnLogin);
        select_character.onClick.AddListener(OnSelectCharacter);
        create_character.onClick.AddListener(OnCreateCharacter);

        DontDestroyOnLoad(network);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnCreateAccount()
	{
        network.CreateAccount(id.text, password.text);
    }

    void OnLogin()
    {
        network.Login(id.text, password.text);
    }

    void OnCreateCharacter()
	{
        network.CreateCharacter(nickname.text);
	}

    void OnSelectCharacter()
	{
        ulong id;
        ulong.TryParse(character_id.text, out id);
        network.SelectCharacter(id);
	}
}

