
public enum NetCalls{
	LOGIN = 0,
	LOADPLAYERS,
	END,
	MSG,
	POSITION,
	PAYER_LEFT

};
public enum ButtonEvents{
	LOGIN_PRESS = 0,//0
	CHATMSG

};
public struct NetMsg{
	public NetCalls type;
	public string data;
	public bool wait;
	public float time;
};
public enum SceneId{
	LOGIN = 0,
	GAME
};
public struct PlayerStats{
	public string name;
	public int id;
};