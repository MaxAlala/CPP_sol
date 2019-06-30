package connection;




import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;
import java.util.logging.Logger;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;


public class GetGlobalPosition { // ++++++++++FINISHED
    private static Logger log = Logger.getLogger(GetResult.class.getName());// DONE +++++++++++++++++!


    //    {"count":172,"next":"http://localhost:8000/account/snippets/?page=4","previous":"http://localhost:8000/account/snippets/?page=2","results":[{"title":"0","owner":"max22","code":"4500","created":"2018-05-26T21:09:56.122319+03:00"},{"title":"2","owner":"max22","code":"4300","created":"2018-05-26T21:09:56.270921+03:00"},{"title":"1","owner":"max22","code":"4400","created":"2018-05-26T21:09:56.433487+03:00"},{"title":"0","owner":"max22","code":"4500","created":"2018-05-26T21:10:00.140744+03:00"},{"title":"2","owner":"max22","code":"4300","created":"2018-05-26T21:10:00.297325+03:00"},{"title":"1","owner":"max22","code":"4400","created":"2018-05-26T21:10:00.463880+03:00"},{"title":"1","owner":"max22","code":"45","created":"2018-05-27T08:47:21.778240+03:00"},{"title":"","owner":"max22","code":"555","created":"2018-05-27T08:49:11.349004+03:00"},{"title":"1","owner":"max22","code":"4","created":"2018-05-27T08:55:04.437853+03:00"},{"title":"1","owner":"max22","code":"4","created":"2018-05-27T08:56:39.989140+03:00"}]}
//"max22"
    // http://localhost:8080/RESTfulExample/json/product/get
    public static int getResults(int game_type) throws ParseException { // amount = number of snippets
        int position = 0;
        try {
//            "http://maximcastlerock2.pythonanywhere.com/snippets/
            URL url = new URL("http://milkywayitis.pythonanywhere.com/api/total_position?game_type="+ game_type +"&username="+ StateOfauthentication.getLogin());
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            conn.setRequestProperty("Accept", "application/json");

            if (conn.getResponseCode() != 200) {
                throw new RuntimeException("Failed : HTTP error code : "
                        + conn.getResponseCode());
            }

            BufferedReader br = new BufferedReader(new InputStreamReader(
                    (conn.getInputStream())));

            String output;
            String input = "";
            System.out.println("Output from Server .... \n");
            log.info("SUCCESS GETTING RESULTS.");
            while ((output = br.readLine()) != null) {
//                System.out.println(output);
                input = input + output;

            }
            input = "{\"results\":["+input+"]}";
            System.out.println(input);

            JsonParser parser = new JsonParser();
            JsonObject mainObject = parser.parse(input).getAsJsonObject();
//            System.out.println(mainObject.toString());
            JsonArray pItem = mainObject.getAsJsonArray("results");






            for (JsonElement user : pItem) {
                JsonObject userObject = user.getAsJsonObject();
//                if (userObject.get("code").getAsInt() == 555) {
//                    System.out.println(userObject.get("owner"));
//                    return;
//                }
                position = userObject.get("position").getAsInt();
//
                log.info("SUCCESS GSON WORK INT GETRESULT.");

            }
            conn.disconnect();

        } catch (MalformedURLException e) {
            e.printStackTrace();
            log.info("ERROR IN TOP.");

        } catch (IOException e) {

            e.printStackTrace();
            log.info("ERROR IN TOP.");
        }
        return position;
    }

}


