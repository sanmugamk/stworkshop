<?php
	require_once('common.inc');
	function workshops_test_request(){
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('workshops_test_request_form');
		return $output;
	}
	function workshops_test_request_form(){
		$hour = array('' => 'Hour');
		$minite = array('' => 'Minute');
		$foss_category = array('' => 'Select FOSS Category');
		$language_array = array('' => 'Select Language');
		$i=0;
		while($i <= 23){
			if($i < 10){
				$hour['0'.$i] = '0'.$i;
			}else{
				$hour[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$minite['0'.$i] = '0'.$i;
			}else{
				$minite[$i] = $i;
			}
			$i++;
		}

		$rows = db_fetch_object($foss_categories);

		while($row = db_fetch_object($foss_categories)) {
			$foss_category[$row->FOSS_Category] = $row->FOSS_Category;
		}
		while($row = db_fetch_object($languages)) {
			$language_array[$row->Language] = $row->Language;
		}
		
		$query = "select distinct name from departments order by name ASC";
		db_set_active('workshop_info');
		$result = db_query($query);
		db_set_active('default');
		$departments = array();
		$departments[''] = '-- select --';
		while($row = db_fetch_object($result)){
			$departments[$row->name] = $row->name;
		}
		$departments['others'] = '...others...';
		$foss_categories[''] = ".. select ..";
		$foss_categories['C'] = "C";
		$foss_categories['C-Plus-Plus'] = "C++";		
		$foss_categories['Java'] = "Java";
		$foss_categories['LaTeX'] = "LaTeX";
		$foss_categories['Linux'] = "Linux";
		// $foss_categories['OpenFOAM'] = "OpenFOAM";
		$foss_categories['PHP-and-MySQL'] = "PHP and MySQL";
		$foss_categories['Python'] = "Python";
		$foss_categories['Scilab'] = "Scilab";
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
		drupal_add_css($path . '/workshops.css');
  drupal_add_js("$(document).ready(function(){ new datepickr('edit-pref-test-date', { dateFormat: 'Y-m-d' }); });", 'inline');	
  		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['state'] = array(
			'#type' => 'select',
			'#title' => t('State'),
			'#required' => TRUE,
			'#options' => get_states(1),
			'#attributes' => array('class' => 'add_reqs_state')
		);
		$form['academic_code'] = array(
			'#type' => 'select',
			'#title' => t('Institution Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array('' => '-- select --'),
			'#attributes' => array('class' => 'add_reqs_academic_code'),
			'#prefix' => '<div class="reqs_academic_code_div">',
			'#suffix' => '</div>'
		);
		$form['department'] = array(
			'#type' => 'select',
			'#title' => t('Department of Participants'),
			'#required' => TRUE,
			'#options' => $departments,
			'#attributes' => array('class' => 'add_reqs_department'),
		);
		$form['invigilator_id'] = array(
			'#type' => 'select',
			'#title' => t('Invigilator Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array('' => '-- select --'),
			'#attributes' => array('class' => 'add_invigilator_sel'),
			'#prefix' => '<div class="add_reqs_invigilator_div">',
			'#suffix' => '</div>'
		);
		$form['pref_test_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Preferred Test Date'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'pref_test_date', 'readonly' => 'readonly')
		);
		$form['pref_test_time'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#title' => t(''),
			'#options' => $hour,
			'#attributes' => array('class' => 'pref_wkshop_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-test-time'>Preferred Test Time:</label>",
		);
		$form['pref_test_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#title' => t(''),
			'#options' => $minite,
			'#attributes' => array('class' => 'pref_wkshop_minute'),
			'#suffix' => "</div>"
		);
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('Foss Category'),
			'#options' => human_text($foss_categories),
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category')
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		drupal_set_message('Please note: Confirm Invigilator availability and acceptance to invigilate before adding his name in this form.');
		return $form;
	}
	
	function workshops_test_request_form_validate($form, &$form_state){
		if($form_state['values']['academic_code'] == ""){
        	form_set_error('academic_code', t('Please select Institution Name'));
        }
		if($form_state['values']['invigilator_id'] == ""){
			form_set_error('invigilator_id', t('Please select Invigilator Name'));
		}
	}
	function workshops_test_request_form_submit($form, &$form_state){
		global $user;
		$pref_test_date = $form_state['values']['pref_test_date'];
		$pref_test_time = $form_state['values']['pref_test_time'].':'.$form_state['values']['pref_test_minute'];
		$query = "insert into  test_requests values('','', '".$form_state['values']['academic_code']."', '".$user->uid."', '".$form_state['values']['department']."', '".$form_state['values']['invigilator_id']."', '".$pref_test_date."','', '".$pref_test_time."', '', '".$form_state['values']['foss_category']."', '')";
		db_set_active('workshop_info');
		if(db_query($query)){
			// mail to Organiser
			//$mailfrom = variable_get('site_mail');
			$mailto = $user->mail;
			$subject = 'Test Request Notification';
			$message = "<p>Dear ".$user->name.",</p><p>Thank you for registering at <a href='http://spoken-tutorial.org'>spoken-tutorial.org</a>.Your Test request is waiting for approval. Once it has been approved, you will receive another e-mail containing information about test confirmation details.</p>";
			workshops_notify($mailto, $subject, $message);
			// send email to Resource person
			//$query = "select rp.rp_fname, u.mail from workshop_info.resource_person rp, CDEEP.users u where u.name in (select resource_person_id from academic_center where academic_code='".$form_state['values']['academic_code']."')";
			$tem = explode('-', $form_state['values']['academic_code']);
			$query = "select rp.rp_fname, u.mail from workshop_info.resource_person rp, CDEEP.users u where FIND_IN_SET('".$tem[0]."', rp.states) > 0 and rp.user_uid=u.uid";
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$mailto = '';
			while($row = db_fetch_object($result)){
				$mailto .= $row->mail.',';
			}
			//$mailfrom = variable_get('site_mail');
			$subject = 'Test Request Notification';
			$message = "<p>Dear Resource-person,</p><p>".$user->name." made a new Test request.</p><p><b>Test Details</b></p><p>Foss category : ".$form_state['values']['foss_category']."</p><p>Preferred Test Date : ".$pref_test_date."</p><p>Preferred Test Time : ".$pref_test_time."</p>";
			//echo $mailto."<br><br>".$subject."<br><br>".$message; die;
			workshops_notify($mailto, $subject, $message);
			
			drupal_set_message('Test Request Added Successfully');
			drupal_set_message('<p>Upgrade the browser before the test with latest version, <br>for instructions <a href="" style="font-size: 16px;"><b><i>Click Here</i></b></a></p>');
			drupal_goto($path = 'workshops/test_request', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Test Request Details');
		}
		db_set_active('default');
		//drupal_goto($path = 'workshops/list_test_request', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}

	function workshops_list_test_request(){
		global $user;
		
		$query = "";
		$count_query = "";
		
		if(in_array('organiser', array_values($user->roles))) {
			$query = "select tr.id, tr.academic_code, tr.department, tr.pref_test_date, tr.pref_test_time from test_requests tr where organiser_id=".$user->uid." and status=0";
			$count_query = "select count(id) from test_requests where organiser_id=".$user->uid." and status=0";
		}
		
		if(in_array('resource_person', array_values($user->roles))) {
			$query = "select states from resource_person where user_uid=".$user->uid;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$state_code_arr = explode(',', $row->states);
		
			$tmp_query_param = "";
			foreach($state_code_arr as $st_code){
				if($tmp_query_param != ""){
					$tmp_query_param .= " or ";
				}
				$tmp_query_param .=	"academic_code like '".$st_code."%'";
			}
			$query = "select tr.id, tr.academic_code, tr.department, tr.pref_test_date, tr.pref_test_time, tr.foss_category from test_requests tr where (".$tmp_query_param.") and status=0";
			$count_query = "select count(id) from test_requests where (".$tmp_query_param.") and status=0";
		}
		if(in_array('organiser', array_values($user->roles))) {
			$header = array(
				array('data'=>'FOSS Category', 'field'=>'tr.foss_category'),
				array('data'=>'Academic Center Code', 'field'=>'tr.academic_code'),
				array('data'=>'Department', 'field'=>'tr.department'),
				array('data'=>'Preferred Date & Time', 'field'=>'tr.pref_test_date', 'sort'=>'desc')
			);
		}
		if(in_array('resource_person', array_values($user->roles))) {
			$header = array(
				array('data'=>'FOSS Category', 'field'=>'tr.foss_category'),
				array('data'=>'Academic Center Code', 'field'=>'tr.academic_code'),
				array('data'=>'Department', 'field'=>'tr.department'),
				array('data'=>'Preferred Date & Time', 'field'=>'tr.pref_test_date','sort'=>'desc')
			);
		}
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		$sno = 1;
		while ($item = db_fetch_object($result)) {
			if(in_array('resource_person', array_values($user->roles))) {
				$row[] = array($item->foss_category, $item->academic_code, $item->department, $item->pref_test_date." ".$item->pref_test_time, l('Confirm', 'workshops/test_confirm/'. $item->id, array('attributes' => array('class' => 'confirm-record', 'title' => 'Confirm'))).l('Reject', 'workshops/test_delete/'. $item->id, array('attributes' => array('class' => 'delete-record', 'title' => 'Reject', 'onclick' => 'return confirm("Are you sure?")'))));
			}else {
				$row[] = array($item->foss_category, $item->academic_code, $item->department, $item->pref_test_date." ".$item->pref_test_time);
			}
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 30);
		}
		return $output;
	}

	function workshops_list_test_approval(){
		global $user;

		$query = "";
		$count_query = "";
		
		if(in_array('organiser', array_values($user->roles))) {
			$query = "select id, test_code, academic_code, cfm_test_date, cfm_test_time, foss_category, status from test_requests where organiser_id=".$user->uid." and status!=0 and status!=3 and status!=4";
			$count_query = "select count(id) from test_requests where organiser_id=".$user->uid." and status!=0 and status!=3 and status!=4";
		}
		
		if(in_array('resource_person', array_values($user->roles))) {
			$query = "select states from resource_person where user_uid=".$user->uid;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$state_code_arr = explode(',', $row->states);
		
			$tmp_query_param = "";
			foreach($state_code_arr as $st_code){
				if($tmp_query_param != ""){
					$tmp_query_param .= " or ";
				}
				$tmp_query_param .=	"academic_code like '".$st_code."%'";
			}
			$query = "select id, test_code, academic_code, cfm_test_date, cfm_test_time, foss_category, status from test_requests where (".$tmp_query_param.") and status!=0 and status!=3 and status!=4";
			//echo $query; die;
			$count_query = "select count(id) from test_requests where (".$tmp_query_param.") and status!=0 and status!=3 and status!=4";
		}
		if(in_array('organiser', array_values($user->roles))) {
			$header = array(
				array('data'=>'Code', 'field'=>'id'),
				array('data'=>'FOSS Category', 'field'=>'foss_category'),
				array('data'=>'Academic Center Code', 'field'=>'academic_code'),
				array('data'=>'Date & Time', 'field'=>'cfm_test_date' , 'sort'=>'desc'),
				array('data'=>'Status', 'field'=>'status')
			);
		}
		if(in_array('resource_person', array_values($user->roles))) {
			$header = array(
				array('data'=>'Code', 'field'=>'id'),
				array('data'=>'FOSS Category', 'field'=>'foss_category'),
				array('data'=>'Academic Center Code', 'field'=>'academic_code'),
				array('data'=>'Date & Time', 'field'=>'cfm_test_date', 'sort'=>'desc'),
				array('data'=>'Status', 'field'=>'status')
			);
		}
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		while ($item = db_fetch_object($result)) {
			$status_data = '';
			if($item->status == 1){
				$status_data = 'Waiting for Invigilator\'s approval';
			}
			if($item->status == 2){
				$status_data = 'Approved';
			}
			if(in_array('resource_person', array_values($user->roles))) {
				$row[] = array($item->test_code, $item->foss_category, $item->academic_code, $item->cfm_test_date." ".$item->cfm_test_time, $status_data, l('Edit', 'workshops/test_edit/'. $item->id, array('attributes' => array('class' => 'edit-record', 'title' => 'Edit'))));
			}else {
				$row[] = array($item->test_code, $item->foss_category, $item->academic_code, $item->cfm_test_date." ".$item->cfm_test_time, $status_data);
			}
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 30);
		}
		return $output;
	}
	
	function workshops_list_test_completed(){
		global $user;
		
		$query = "";
		$count_query = "";
		
		if(in_array('organiser', array_values($user->roles))) {
			$query = "select id, test_code, academic_code, cfm_test_date, cfm_test_time, foss_category from test_requests where organiser_id=".$user->uid." and status=4";
			$count_query = "select count(id) from test_requests where organiser_id=".$user->uid." and status=4";
		}
		
		if(in_array('resource_person', array_values($user->roles))) {
			$query = "select states from resource_person where user_uid=".$user->uid;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$state_code_arr = explode(',', $row->states);
			$tmp_query_param = "";
			foreach($state_code_arr as $st_code){
				if($tmp_query_param != ""){
					$tmp_query_param .= " or ";
				}
				$tmp_query_param .=	"academic_code like '".$st_code."%'";
			}
			$query = "select id, test_code, academic_code, cfm_test_date, cfm_test_time, foss_category from test_requests where (".$tmp_query_param.") and status=4";
			//echo $query; die;
			$count_query = "select count(id) from test_requests where (".$tmp_query_param.") and status=4";
		}
		$header = array(
			array('data'=>'Code', 'field'=>'id'),
			array('data'=>'FOSS', 'field'=>'foss_category'),
			array('data'=>'Academic Code', 'field'=>'academic_code'),
			array('data'=>'Date & Time', 'field'=>'cfm_test_date', 'sort'=>'desc')
		);
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($item->test_code, human_text($item->foss_category), $item->academic_code, $item->cfm_test_date." ".$item->cfm_test_time, l('View', 'workshops/test_view/'. $item->id, array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 30);
		}
		return $output;
	}

	function workshops_test_confirm($id = NULL){
		if($id){
			$query = "select * from {test_requests} WHERE id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$item = db_fetch_object($result);

			$output = '<div class="form-item" id="academic-details"><table>';
			$output .= '<tr><td>Academic Center Code</td><td>: '.$item->academic_code.'</td></tr>';
			// $output .= '<tr><td>Language</td><td>: '.$item->pref_language.'</td></tr>';
			$output .= '<tr><td>FOSS Category</td><td>: '.$item->foss_category.'</td></tr>';
			$output .= '<tr><td>Preferred Date & Time</td><td>: '.$item->pref_test_date.' '.$item->pref_test_time.'</td></tr>';
			$output .= '</table></div>';
			$output .= drupal_get_form('workshops_test_confirm_form', $id, $item->pref_test_date, $item->pref_test_time);
			return $output;
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_test_delete($id = NULL){
		if($id){
			$query = "delete from test_requests where id=".$id;
			db_set_active('workshop_info');
			db_query($query);
			db_set_active('default');
			
			drupal_set_message('Test request removed successfully!');
			drupal_goto('workshops/list_test_request');
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_test_confirm_form($form_state, $id, $date_val, $time_val){
  $path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
		drupal_add_css($path . '/workshops.css');
  drupal_add_js("$(document).ready(function(){ new datepickr('edit-cfm-test-date', { dateFormat: 'Y-m-d' }); });", 'inline');	
		$time_arr = explode(':', $time_val);
		$hour = array();
		$minite = array();
		$i=0;
		while($i <= 23){
			if($i < 10){
				$hour['0'.$i] = '0'.$i;
			}else{
				$hour[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$minite['0'.$i] = '0'.$i;
			}else{
				$minite[$i] = $i;
			}
			$i++;
		}
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['id'] = array(
			'#type' => 'hidden',
			'#value' => $id,
		);
		$form['cfm_test_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Confirm Test Date'),
			'#required' => TRUE,
			'#value' => $date_val,
			'#attributes' => array('class' => 'cfm_test_date', 'readonly' => 'readonly')
		);
		$form['cfm_test_time'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $hour,
			'#default_value' => $time_arr[0],
			'#attributes' => array('class' => 'pref_wkshop_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-test-time'>Confirm Test Time:<span class='form-required' title='This field is required.'>*</span></label>",
		);
		$form['cfm_test_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#default_value' => $time_arr[1],
			'#attributes' => array('class' => 'pref_wkshop_minute'),
			'#suffix' => "</div>"
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		return $form;
	}
	function workshops_test_confirm_form_validate($form, &$form_state){
		 // if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
		 // 	form_set_error('phone number', t('Invalid Phone number'));
		 // }
		 // if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
		 // 	form_set_error('email', t('Invalid Email'));
		 // }
  		return;
	}
	function workshops_test_confirm_form_submit($form, &$form_state){
		global $user;
		$cfm_test_date = $_POST['cfm_test_date'];
		$cfm_test_time = $_POST['cfm_test_time'].':'.$_POST['cfm_test_minute'].':00';
		$test_code = 'TC-'.$form_state['values']['id'];
		$query = "update {test_requests} set test_code='".$test_code."', cfm_test_date='".$cfm_test_date."', cfm_test_time='".$cfm_test_time."', status=1 where id=".$form_state['values']['id'];
		db_query($query);
		$query = "select ac.institution_name, tr.department, tr.cfm_test_date, tr.cfm_test_time, tr.foss_category, tr.organiser_id, tr.invigilator_id from academic_center ac, test_requests tr where tr.academic_code=ac.academic_code and tr.id=".$_POST['id'];
		db_set_active('workshop_info');
		if($result = db_query($query)){
		    db_set_active('default');
			drupal_set_message('Test Confirm Details Added Successfully');
			$data = db_fetch_object($result);

			// send email to Organiser
			$query = "select name, mail from users where uid='".$data->organiser_id."'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $row->mail;
			//$mailfrom = variable_get('site_mail');
			$subject = 'Test request waiting for Invigilator acceptance';
			$message = "<p>Dear ".$row->name.",</p><p> Your new Test request has been approved by Resource person. Now it is  waiting for invigilator's acceptance. Once it accepted, you will receive a test comfirmation email.</p>";
			echo $mailto."<br><br>".$subject."<br><br>".$message;
			workshops_notify( $mailto, $subject, $message);
			
			// send email to Invigilator
			$query = "select name, mail from users where uid='".$data->invigilator_id."'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $row->mail;
			//$mailfrom = variable_get('site_mail');
			$subject = 'Test request waiting for your approval';
			$message = "<p>Dear ".$row->name.",</p><p>The following test is waiting for your acceptance,  </p>";
			$message .= "<table><tr><td>Institution Name</td><td>: ".$data->institution_name."</td></tr>";
			$message .= "<tr><td>Department</td><td>: ".$data->department."</td></tr>";
			$message .= "<tr><td>Foss Category </td><td>: ".$data->foss_category ."</td></tr>";
			$message .= "<tr><td>Test date & time</td><td>: ".$data->cfm_test_date."  ".$data->cfm_test_time."</td></tr></table>";
			$message .= "<p>Please use the following URL to accept/reject, http://spoken-tutorial.org/workshops/invigilate_pending_test_requests</p>";
			echo $mailto."<br><br>".$subject."<br><br>".$message;
			workshops_notify( $mailto, $subject, $message);

			drupal_goto($path = 'workshops/list_test_request', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Test Confirm Details');
		}
		db_set_active('default');
	}
	function workshops_test_view($id = NULL){
	 	if($id){
	 		$query = "select tr.*, td.no_of_participants, td.invigilator from test_requests tr, test_details td where tr.id=".$id." and td.test_code=tr.test_code";
	 		// echo $query;
	 		// exit;
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');
	 		if($result){
	 			$item = db_fetch_object($result);
	 			$output = '<table width="100%" cellpadding="6">';
	 			$output .= '<tr><td><b>Test Code</b></td><td><b>'.$item->test_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Foss Category</b></td><td><b>'.$item->foss_category.'</b></td></tr>';
	 			$output .= '<tr><td><b>Date & Time</b></td><td><b>'.$item->cfm_test_date.' &nbsp;'.$item->cfm_test_time.'</b></td></tr>';
	 			$output .= '<tr><td><b>Invigilator</b></td><td><b>'.$item->invigilator.'</b></td></tr>';
	 			$output .= '<tr><td><b>Participants Count</b></td><td><b>'.$item->no_of_participants.'</b></td></tr>';
	 			$output .= '<tr><td colspan="2"><hr /></td></tr>';
	 			$query = "select * from {academic_center} where academic_code='".$item->academic_code."'";
	 			db_set_active('workshop_info');
	 			$result1 = db_query($query);
	 			db_set_active('default');
	 			$coll_detail = db_fetch_object($result1);

	 			$output .= '<tr><td><b>State Code</b></td><td><b>'.$coll_detail->state_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Academic Center Code</b></td><td><b>'.$coll_detail->academic_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Institution Name</b></td><td><b>'.$coll_detail->institution_name.'</b></td></tr>';
	 			$output .= '<tr><td><b>Department</b></td><td><b>'.$item->department.'</b></td></tr>';
	 			$output .= '<tr><td><b>Street</b></td><td><b>'.$coll_detail->street.'</b></td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td><b>'.$coll_detail->city.'</b></td></tr>';
	 			$output .= '<tr><td><b>Pin Code</b></td><td><b>'.$coll_detail->pincode.'</b></td></tr>';
	 			$output .= '</table>';

	 			return $output;
	 		}else {
	 			drupal_set_message('Error accessing database workshop_info');
	 		}
	 	}else{
	 		return "<p>Sorry, no data found.</p>";
	 	}
	}

	function workshops_test_edit($id = NULL){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
		drupal_add_css($path . '/workshops.css');
  		drupal_add_js("$(document).ready(function(){ new datepickr('edit-cfm-test-date', { dateFormat: 'Y-m-d' }); });", 'inline');	
		if($id){
			$query = "select * from {test_requests} WHERE id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			$item = db_fetch_object($result);
			db_set_active('default');
			//$query = "select * from {academic_center} WHERE academic_code='". $item->academic_code."'";

			//$result = db_query($query);
			//$academic_item = db_fetch_object($result);
			$output = '<div class="form-item" id="academic-details"><table width="100%" cellpadding="6">';
			$output .= '<tr><td>Academic Center Code</td><td>: '.$item->academic_code.'</td></tr>';
			$output .= '<tr><td>FOSS Category</td><td>: '.$item->foss_category.'</td></tr>';
			$output .= '<tr><td>Preferred Date & Time</td><td>: '.$item->cfm_test_date.' '.$item->cfm_test_time.'</td></tr>';
			$output .= '</table></div>';
			$output .= drupal_get_form('workshops_test_edit_form', $id, $item->cfm_test_date, $item->cfm_test_time, $item->organiser_id);
			return $output;
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_test_edit_form($form_state, $id, $date_val, $time_val, $oid){
		$time_arr = explode(':', $time_val);
		$hour = array();
		$minite = array();
		$i=0;
		while($i <= 23){
			if($i < 10){
				$hour['0'.$i] = '0'.$i;
			}else{
				$hour[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$minite['0'.$i] = '0'.$i;
			}else{
				$minite[$i] = $i;
			}
			$i++;
		}
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['id'] = array(
			'#type' => 'hidden',
			'#title' => t('test request id'),
			'#value' => $id,
		);
		$form['oid'] = array(
			'#type' => 'hidden',
			'#title' => t('organiser id'),
			'#value' => $oid,
		);
		$form['cfm_test_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Confirmed Test Date'),
			'#required' => TRUE,
			'#size' => 30,
			'#value' => $date_val,
			'#attributes' => array('class' => 'cfm_test_date', 'readonly' => 'readonly')
		);
		$form['cfm_test_time'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $hour,
			'#default_value' => $time_arr[0],
			'#attributes' => array('class' => 'pref_test_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-test-time'>Confirmed Test Time:<span class='form-required' title='This field is required.'>*</span></label>",
		);
		$form['cfm_test_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#default_value' => $time_arr[1],
			'#attributes' => array('class' => 'pref_test_minute'),
			'#suffix' => "</div>"
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		return $form;
	}
	
	function workshops_test_edit_form_submit($form, &$form_state){
		global $user;
		$cfm_test_date = $_POST['cfm_test_date'];
		$cfm_test_time = $_POST['cfm_test_time'].':'.$_POST['cfm_test_minute'].':00';
		$query = "update {test_requests} set cfm_test_date='".$cfm_test_date."', cfm_test_time='".$cfm_test_time."', status=1 where id=".$form_state['values']['id'];
		db_set_active('workshop_info');
		db_query($query);
		db_set_active('default');
		$query = "select ac.institution_name, tr.department, tr.cfm_test_date, tr.cfm_test_time, tr.foss_category, tr.organiser_id, tr.invigilator_id from academic_center ac, test_requests tr where tr.academic_code=ac.academic_code and tr.id=".$_POST['id'];
		if(db_query($query)){		
			// send email to Organiser
			$query = "select name, mail from users where uid='".$data->organiser_id."'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $row->mail;
			//$mailfrom = variable_get('site_mail');
			$subject = 'Test request waiting for Invigilator acceptance';
			$message = "<p>Dear ".$row->name.",</p><p> Your new Test request has been approved by Resource person. Now it is  waiting for invigilator's acceptance. Once it accepted, you will receive a test comfirmation email.</p>";
			echo $mailto."<br><br>".$subject."<br><br>".$message;
			workshops_notify( $mailto, $subject, $message);
			
			// send email to Invigilator
			$query = "select name, mail from users where uid='".$data->invigilator_id."'";
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $row->mail;
			//$mailfrom = variable_get('site_mail');
			$subject = 'Test request waiting for your approval';
			$message = "<p>Dear ".$row->name.",</p><p>The following test is waiting for your acceptance,  </p>";
			$message .= "<table><tr><td>Institution Name</td><td>: ".$data->institution_name."</td></tr>";
			$message .= "<tr><td>Department</td><td>: ".$data->department."</td></tr>";
			$message .= "<tr><td>Foss Category </td><td>: ".$data->foss_category ."</td></tr>";
			$message .= "<tr><td>Test date & time</td><td>: ".$data->cfm_test_date."  ".$data->cfm_test_time."</td></tr></table>";
			$message .= "<p>Please use the following URL to accept/reject, http://spoken-tutorial.org/workshops/invigilate_pending_test_requests</p>";
			echo $mailto."<br><br>".$subject."<br><br>".$message;
			workshops_notify( $mailto, $subject, $message);


			drupal_set_message('Test Confirmation Added Successfully');
			drupal_goto($path = 'workshops/list_test_approval', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Test Confirmation Details');
		}
		db_set_active('default');
	}
	
	function workshops_admin_test_approval(){
		global $user;
		$query = "select * from {test_requests} where status=1 order by test_code";
		$header = array('', 'FOSS Category', 'Academic Code',  'Date & Time', '');
		$tablesort = tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query . $tablesort, 30, 0, NULL);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, human_text($item->foss_category), $item->academic_code, $item->cfm_test_date." ".$item->cfm_test_time, l('View', 'workshops/test_view/'. $item->id, array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output = theme('table', $header, $row);
			$output .= theme('pager');
		}
		return $output;
	}
?>
