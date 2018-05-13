using UnityEngine;
using UnityEngine.UI;
public class S_LoginScene : iScene {
	public S_LoginScene(GameObject target) : base(target){
		NetworkController nc = GameObject.Find("GameMaster").GetComponent<NetworkController>();
		nc.RegisterCallback ((int)(NetCalls.LOGIN), new LogIn ());
		nc.RegisterCallback ((int)(NetCalls.LOADPLAYERS), new LoadPlayer());
		//GameObject.Find("LogInText").GetComponent<Text>().text = "RUSKI";
		//GameObject.Find("IpText").GetComponent<Text>().text = "51.37.158.114";
		//GameObject.Find("PortText").GetComponent<Text>().text = "5555";
	}
	public override void Update ()
	{

	}
	public override void ButtonHandle (ButtonEvents be){
		if (be == ButtonEvents.LOGIN_PRESS) {
			LogIn ();
		}
	}
	public void LogIn(){
		NetworkController nc = GameObject.Find("GameMaster").GetComponent<NetworkController>();
		string ip = GameObject.Find("IpText").transform.FindChild("Text").GetComponent<Text>().text;
		string port = GameObject.Find("PortText").transform.FindChild("Text").GetComponent<Text>().text;
		nc.Connect (ip, int.Parse (port));

		//get string from input field
		//get the text from input field
		string name;
		name = GameObject.Find("NameText").transform.FindChild("Text").GetComponent<Text>().text;
		string login = GameObject.Find("LogInText").transform.FindChild("Text").GetComponent<Text>().text;
		nc.Send (NetCalls.LOGIN, (name+'|'+login), true, 2.0f);
	}
	public override void End ()
	{
		_sceneObj.SetActive (false);
	}
	public override void Start ()
	{
		_sceneObj.SetActive (true);
	}

}
