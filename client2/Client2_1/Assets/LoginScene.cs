using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class LoginScene : MonoBehaviour
{
    public Button create_account;
    public Button login;

    public TMP_InputField id;
    public TMP_InputField password;

    public Network network;

    // Start is called before the first frame update
    void Start()
    {
        create_account.onClick.AddListener(OnCreateAccount);
        create_account.onClick.AddListener(OnLogin);

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

}

