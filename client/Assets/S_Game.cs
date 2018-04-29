using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class S_Game : iScene {

	public S_Game(GameObject target):base(target){
		NetworkController nc = GameObject.Find("GameMaster").GetComponent<NetworkController>();
		nc.RegisterCallback ((int)(NetCalls.MSG), new M_ChatMSg ());
		nc.RegisterCallback ((int)(NetCalls.POSITION), new M_Position());
		nc.RegisterCallback ((int)(NetCalls.PAYER_LEFT), new M_PlayerLeft());
		Physics.gravity = new Vector3 (0, -500.0f, 0);
	}

	public override void Start ()
	{
		Debug.Log("START IS CALLED START IS CALLED START IS CALLED START IS CALLED ");
		_sceneObj.SetActive (true);
		Transform camera = _sceneObj.transform.FindChild ("Camera");
		GameObject player = GameObject.Find ("GameMaster").GetComponent<PlayerManager> ().GetMyPlayerObj ();
		PlayerController pc;

		pc = player.AddComponent(typeof(PlayerController)) as PlayerController;
		pc._camera = camera;

	}

	public override void End ()
	{
		_sceneObj.SetActive (false);
	}

	public override void ButtonHandle (ButtonEvents be)
	{
		if (be == ButtonEvents.CHATMSG) {
			
			string text = GameObject.Find ("GameMaster").GetComponent<PlayerManager> ().GetMyPlayer ().GetStats ().name;
			text = text + ": " + GameObject.Find("ChatInput").transform.FindChild("InputField").transform.FindChild("Text").GetComponent<Text>().text;
			//Send ("-m-"+getLenStr(text)+text);
			//GameObject.Find("MainText").GetComponent<Text>().text += "\n"+text;
			GameObject.Find ("GameMaster").GetComponent<NetworkController>().Send(NetCalls.MSG, text, false, 0.0f);
		}
	}

	public override void Update ()
	{
		
	}
}
