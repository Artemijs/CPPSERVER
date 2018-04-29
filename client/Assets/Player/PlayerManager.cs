using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
create player
manage all players

*/
public class PlayerManager : MonoBehaviour {
	List<GameObject> _allPlayers;
	public GameObject _pPrefab;
	public int _playerId;
	GameObject _temp;
	void Awake(){
		_allPlayers = new List<GameObject> ();		
	}
	// Use this for initialization
	void Start () {
		_temp = GameObject.Find("Temp");
	}
	public void SetMyPlayerId(int mId){
		_playerId = mId;
	}
	public int CreatePlayer(PlayerStats ps){
		GameObject np = Instantiate (_pPrefab, new Vector3 (0, 0, 0), _pPrefab.transform.rotation);
		Player pScript = np.GetComponent<Player> ();
		pScript.SetStats (ps);
		_allPlayers.Add (np);
		return _allPlayers.Count - 1;
	}
	public void SortPlayerArr(){
		GameObject temp;
		for (int i = 0; i < _allPlayers.Count; i++) {
			PlayerStats ps = _allPlayers [i].GetComponent<Player> ().GetStats ();
			if (ps.id != i) {
				temp = _allPlayers [ps.id];
				_allPlayers [ps.id] = _allPlayers [i];
				_allPlayers [i] = temp;
			}
		}
	}
	public Player GetMyPlayer(){
		return _allPlayers [_playerId].GetComponent<Player>();
	}
	public GameObject GetMyPlayerObj(){
		return _allPlayers [_playerId];
	}
	// Update is called once per frame
	void Update () {
		
	}
	public void SetPlayerBasic(int id, Vector3 pos, Vector3 angles){
		Debug.Log (pos);
		GameObject target;
		target = _allPlayers [id];
		Player p = target.GetComponent<Player> ();

		//target.transform.position = pos;
		Quaternion rotation = target.transform.localRotation;
		//Debug.Log (angles);
		rotation.eulerAngles = angles;
		target.transform.localRotation = rotation;
		target.transform.position = pos;
	}
	public void UpdatePlayerBasic(int id, Vector3 pos, Vector3 angles){
		Debug.Log (pos);
		GameObject target;
		if (id != _playerId)
			target = _allPlayers [id];
		else
			target = _temp;
		Player p = target.GetComponent<Player> ();

		//target.transform.position = pos;
		Quaternion rotation = target.transform.localRotation;
		//Debug.Log (angles);
		rotation.eulerAngles = angles;
		target.transform.localRotation = rotation;
		p.Move (pos);
	}
	public void RemovePlayer(int id){
		GameObject go = _allPlayers [id];
		for (int i = id; i < _allPlayers.Count - 1; i++) {
			Player p = _allPlayers [i+1].GetComponent<Player>();
			Debug.Log (p.GetStats ().id.ToString ());
			PlayerStats s = p.GetStats ();
			s.id--;
			p.SetStats (s);
			Debug.Log (p.GetStats ().id.ToString ());
			_allPlayers [i] = _allPlayers [i + 1];
		}
		Debug.Log (_allPlayers [_playerId].GetComponent<Player> ().GetStats ().id.ToString ());
		_allPlayers.RemoveAt (_allPlayers.Count-1);
		if (_playerId > id) {
			_playerId--;
		}
		Destroy (go);
	}

}
