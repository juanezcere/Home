package zabavy.zabavyhome;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v4.graphics.drawable.RoundedBitmapDrawable;
import android.support.v4.graphics.drawable.RoundedBitmapDrawableFactory;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.Toast;
import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;

import java.util.List;
import cz.msebera.android.httpclient.Header;

public class NewActivity extends AppCompatActivity {

    WifiManager wifiManager;
    boolean maxState = true;
    Spinner cbWifi;
    EditText txtPass, txtNombre, txtDescripcion;
    ImageView imgImagen, imgColor;
    Button btnGuardar, btnCancelar;
    String[] datos;
    dbZabavy db;
    String IP = "10.0.0.1", ID = "";
    int imagen = 0, selectedImg = R.drawable.lights64, color = 0, selectedColor = R.color.white, oldNet;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new);
        db = new dbZabavy(getApplicationContext());
        cbWifi = (Spinner) findViewById(R.id.cbWifi);
        txtPass = (EditText) findViewById(R.id.txtPass);
        txtNombre = (EditText) findViewById(R.id.txtNombre);
        txtDescripcion = (EditText) findViewById(R.id.txtDescripcion);
        imgImagen = (ImageView) findViewById(R.id.imgImagen);
        imgColor = (ImageView) findViewById(R.id.imgColor);
        btnGuardar = (Button) findViewById(R.id.btnGuardar);
        btnCancelar = (Button) findViewById(R.id.btnCancelar);
        imgImagen.setImageResource(R.drawable.lights64);
        Drawable originalDrawable = imgImagen.getDrawable();
        Bitmap originalBitmap = ((BitmapDrawable) originalDrawable).getBitmap();
        RoundedBitmapDrawable roundedDrawable = RoundedBitmapDrawableFactory.create(getResources(), originalBitmap);
        roundedDrawable.setCornerRadius(originalBitmap.getHeight());
        imgImagen.setImageDrawable(roundedDrawable);
        imgImagen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                imagen++;
                if(imagen > 11) imagen = 0;
                switch (imagen){
                    case 0:
                        selectedImg = R.drawable.lights64;
                        break;
                    case 1:
                        selectedImg = R.drawable.bathroom;
                        break;
                    case 2:
                        selectedImg = R.drawable.stairs;
                        break;
                    case 3:
                        selectedImg = R.drawable.office;
                        break;
                    case 4:
                        selectedImg = R.drawable.diningroom;
                        break;
                    case 5:
                        selectedImg = R.drawable.exterior;
                        break;
                    case 6:
                        selectedImg = R.drawable.garage;
                        break;
                    case 7:
                        selectedImg = R.drawable.kitchen;
                        break;
                    case 8:
                        selectedImg = R.drawable.livingroom;
                        break;
                    case 9:
                        selectedImg = R.drawable.room;
                        break;
                    case 10:
                        selectedImg = R.drawable.sleepingroom;
                        break;
                    case 11:
                        selectedImg = R.drawable.washingroom;
                        break;
                }
                imgImagen.setImageResource(selectedImg);
                Drawable originalDrawable = imgImagen.getDrawable();
                Bitmap originalBitmap = ((BitmapDrawable) originalDrawable).getBitmap();
                RoundedBitmapDrawable roundedDrawable = RoundedBitmapDrawableFactory.create(getResources(), originalBitmap);
                roundedDrawable.setCornerRadius(originalBitmap.getHeight());
                imgImagen.setImageDrawable(roundedDrawable);
            }
        });
        imgColor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                color++;
                if(color > 14) color = 0;
                switch (color){
                    case 0:
                        selectedColor = R.color.white;
                        break;
                    case 1:
                        selectedColor = R.color.blue;
                        break;
                    case 2:
                        selectedColor = R.color.darkblue;
                        break;
                    case 3:
                        selectedColor = R.color.darkgreen;
                        break;
                    case 4:
                        selectedColor = R.color.darkorange;
                        break;
                    case 5:
                        selectedColor = R.color.darkpurple;
                        break;
                    case 6:
                        selectedColor = R.color.darkred;
                        break;
                    case 7:
                        selectedColor = R.color.green;
                        break;
                    case 8:
                        selectedColor = R.color.orange;
                        break;
                    case 9:
                        selectedColor = R.color.purple;
                        break;
                    case 10:
                        selectedColor = R.color.red;
                        break;
                    case 11:
                        selectedColor = R.color.trans;
                        break;
                    case 12:
                        selectedColor = R.color.colorAccent;
                        break;
                    case 13:
                        selectedColor = R.color.colorPrimaryDark;
                        break;
                    case 14:
                        selectedColor = R.color.colorShadow;
                        break;
                }
                imgColor.setBackgroundColor(getResources().getColor(selectedColor));
            }
        });
        btnGuardar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if((txtPass.getText().length() == 0) && (maxState)){
                    Toast.makeText(getBaseContext(), getString(R.string.llenar) + " " + getString(R.string.pass).toLowerCase(), Toast.LENGTH_LONG).show();
                } else if((txtPass.getText().length() < 8) && (maxState)){
                    Toast.makeText(getBaseContext(), getString(R.string.ocho), Toast.LENGTH_LONG).show();
                }
                if(txtNombre.getText().length() == 0){
                    Toast.makeText(getBaseContext(), getString(R.string.llenar) + " " + getString(R.string.nombre).toLowerCase(), Toast.LENGTH_LONG).show();
                }
                if(txtDescripcion.getText().length() == 0){
                    Toast.makeText(getBaseContext(), getString(R.string.llenar) + " " + getString(R.string.descripcion).toLowerCase(), Toast.LENGTH_LONG).show();
                }
                if((txtPass.getText().length() >= 8) && (txtNombre.getText().length() > 0) && (txtDescripcion.getText().length() > 0) && (maxState)){
                    sendConfig(IP);
                    wifiManager.enableNetwork(oldNet, true);
                    finish();
                }
                if((txtNombre.getText().length() > 0) && (txtDescripcion.getText().length() > 0) && (!maxState)){
                    update();
                    finish();
                }
            }
        });
        btnCancelar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
        Bundle bundle = getIntent().getExtras();
        try {
            ID = bundle.getString("ID");
            maxState = bundle.getBoolean("NEW");
        } catch (java.lang.NullPointerException e){
            maxState = true;
        }
        if(maxState) {
            wifiManager = (WifiManager) getBaseContext().getSystemService(Context.WIFI_SERVICE);
            boolean wifiState = true;
            if (!wifiManager.isWifiEnabled()) {
                wifiState = false;
                wifiManager.setWifiEnabled(true);
            }
            List<ScanResult> wireless = wifiManager.getScanResults();
            while (wireless.isEmpty()) {
                SystemClock.sleep(100);
                wireless = wifiManager.getScanResults();
            }
            oldNet = wifiManager.getConnectionInfo().getNetworkId();
            wifiManager.disableNetwork(oldNet);
            boolean State = false;
            for (ScanResult scan : wireless) {
                if (scan.SSID.contains("ZABAVY HOME")) {
                    WifiConfiguration config = new WifiConfiguration();
                    config.SSID = "\"" + scan.SSID + "\"";
                    config.BSSID = scan.BSSID;
                    config.priority = 1;
                    config.preSharedKey = "\"12345678\"";
                    config.status = WifiConfiguration.Status.ENABLED;
                    config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                    config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
                    config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
                    config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
                    config.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
                    int id = wifiManager.addNetwork(config);
                    wifiManager.enableNetwork(id, true);
                    wifiManager.saveConfiguration();
                    State = true;
                }
            }
            if (!State) {
                Toast.makeText(getBaseContext(), getString(R.string.noencontrada), Toast.LENGTH_LONG).show();
                wifiManager.enableNetwork(oldNet, true);
                wifiManager.setWifiEnabled(wifiState);
                finish();
            }
            maxState = true;
        } else {
            String[] data = db.readDB(Long.valueOf(ID));
            txtNombre.setText(data[2]);
            txtDescripcion.setText(data[3]);
            imgImagen.setImageResource(Integer.valueOf(data[1]));
            selectedImg = Integer.valueOf(data[1]);
            imgColor.setBackgroundColor(getResources().getColor(Integer.valueOf(data[6])));
            selectedImg = Integer.valueOf(data[6]);
            IP = data[4];
        }
        getDevice(IP);
    }

    private void getDevice(String Direccion){
        AsyncHttpClient client = new AsyncHttpClient();
        String URL = "http://" + Direccion + "/wifis";
        client.get(URL, new AsyncHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                SystemClock.sleep(1000);
                if (statusCode == 200) {
                    String respuesta = new String(responseBody); //"{10}[Red 0][Red 1][Red 2][Red 3][Red 4][Red 5][Red 6][Red 7][Red 8][Red 9];";
                    int cant = Integer.valueOf(respuesta.substring(respuesta.indexOf("{") + 1, respuesta.indexOf("}")));
                    if (cant > 0) {
                        datos = new String[cant];
                        for (int i = 0; i < cant; i++) {
                            datos[i] = respuesta.substring(respuesta.indexOf("[") + 1, respuesta.indexOf("]"));
                            respuesta = respuesta.substring(respuesta.indexOf("]") + 1);
                        }
                        ArrayAdapter<String> adaptador = new ArrayAdapter<>(getBaseContext(), android.R.layout.simple_spinner_dropdown_item, datos);
                        cbWifi.setAdapter(adaptador);
                    }
                } else {
                    Toast.makeText(getBaseContext(), getString(R.string.nodevice), Toast.LENGTH_LONG).show();
                }
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
                Toast.makeText(getBaseContext(), getString(R.string.failure), Toast.LENGTH_LONG).show();
            }
        });
    }

    private void insert(){
        db.insertDB(selectedImg, txtNombre.getText().toString(), txtDescripcion.getText().toString(), IP, false, selectedColor);
    }

    private void update(){
        db.updateDB(Integer.valueOf(ID), selectedImg, txtNombre.getText().toString(), txtDescripcion.getText().toString(), IP, false, selectedColor, 0);
    }

    private void sendConfig(String Direccion){
        AsyncHttpClient client = new AsyncHttpClient();
        String URL = "http://" + Direccion + "/save";
        RequestParams Parametros = new RequestParams();
        Parametros.put("s", cbWifi.getSelectedItem().toString());
        Parametros.put("p", txtPass.getText().toString());
        client.get(URL, Parametros, new AsyncHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, byte[] responseBody) {
                SystemClock.sleep(1000);
                if (statusCode == 200) {
                    IP = new String(responseBody);
                    if(maxState){
                        insert();
                    } else {
                        update();
                    }
                } else {
                    Toast.makeText(getBaseContext(), getString(R.string.nodevice), Toast.LENGTH_LONG).show();
                }
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, byte[] responseBody, Throwable error) {
                Toast.makeText(getBaseContext(), getString(R.string.failure), Toast.LENGTH_LONG).show();
            }
        });
    }
}
