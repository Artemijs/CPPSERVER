using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class M_ChatMSg : iBack {
	public M_ChatMSg():base(){
	
	}
	public override void Start (string message)
	{
		Debug.Log (message);
		message = message.Remove (0, 4);
		GameObject.Find ("MainText").GetComponent<Text> ().text += "\n" + message;
	}

}
